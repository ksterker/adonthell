/*
 $Id: diskwriter_xml.cc,v 1.4 2006/10/09 05:59:22 ksterker Exp $
 
 Copyright (C) 2006 Kai Sterker <kaisterker@linuxgames.com>
 Part of the Adonthell Project http://adonthell.linuxgames.com
 
 Adonthell is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 Adonthell is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Adonthell; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * @file base/diskwriter_xml.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief Read/write xml-formatted data files.
 */

#include <sstream>
#include <vector>
#include <libxml/parser.h>

#include "base/diskwriter_xml.h"

using base::disk_writer_xml;
using base::flat;

/// for converting binary to ascii
char *disk_writer_xml::Bin2Hex = "0123456789ABCDEF";

/// Name of xml data file root node
#define XML_ROOT_NODE "Data"

// definitions for error checking when parsing ints and uints
#define INT8_MAX      127
#define INT16_MAX     32767
#define INT32_MAX     2147483647

#define INT8_MIN      (-INT8_MAX-1)
#define INT16_MIN     (-INT16_MAX-1)
#define INT32_MIN     (-INT32_MAX-1)

#define UINT8_MAX      0xFF
#define UINT16_MAX     0xFFFF
#define UINT32_MAX     0xFFFFFFFFU

/**
 * Context for the sax parser
 */
struct data_sax_context 
{
    /**
     * create a new parser context
     * @param record the record to be loaded
     */
    data_sax_context (base::flat *record)
    {
        Record = record;
        State = disk_writer_xml::UNDEF;
        Stack.push_back (this);
    }
    
    /// Storage for parameters
    base::flat *Record;
    /// id attribute of current element
    std::string Id;
    /// value of primitive element
    std::string Value;
    /// type of current element
    flat::data_type Type;
    /// current state of sax parser
    u_int8 State;
    /// parent of this context
    static std::vector<data_sax_context*> Stack;
};

// context stack
std::vector<data_sax_context*> data_sax_context::Stack;

// safely convert string to unsigned integer
static u_int32 string_to_uint (const char* value, const u_int32 & max)
{
    char *end = NULL;
    u_int32 intval = (u_int32) strtoul (value, &end, 10);
    
    // parsing okay?
    if (*end == '\0') 
    {
        // in range?
        if (intval > max)
        {
            fprintf(stderr, "*** string_to_uint: integer overflow: value '%i' > max '%i'!\n", intval, max);
            return max;
        }
    }
    else 
    {
        // value is not an integer
        fprintf (stderr, "*** string_to_uint: Can't convert '%s' to unsigned integer!\n", value);
        return 0;
    }    
    
    return intval;
}

// safely convert string to signed integer
static s_int32 string_to_sint (const char* value, const s_int32 & min, const s_int32 & max)
{
    char *end = NULL;
    s_int32 intval = (s_int32) strtol (value, &end, 10);
    
    // parsing okay?
    if (*end == '\0') 
    {
        // in range?
        if (intval < min)
        {
            fprintf(stderr, "*** string_to_sint: integer underflow: value '%i' < min '%i'!\n", intval, min);
            return min;
        }
        
        if (intval > max)
        {
            fprintf(stderr, "*** string_to_sint: integer overflow: value '%i' > max '%i'!\n", intval, max);
            return max;
        }
    }
    else 
    {
        // value is not an integer
        fprintf (stderr, "*** string_to_sint: Can't convert '%s' to signed integer!\n", value);
        return -1;
    }    
    
    return intval;
}

// convert primitive params
static void param_to_value (const data_sax_context *context)
{
    const char *value = context->Value.c_str();
    
    switch (context->Type)
    {
        case flat::T_BLOB:
        {
            // TODO
            break;
        }
        case flat::T_BOOL:
        {
            context->Record->put_bool (context->Id, string_to_sint (value, 0, 1) == 1);
            break;
        }
        case flat::T_CHAR:
        {
            context->Record->put_char (context->Id, value[0]);
            break;
        }
        case flat::T_DOUBLE:
        {
            context->Record->put_double (context->Id, strtod (value, NULL));
            break;
        }
        case flat::T_FLOAT:
        {
            context->Record->put_float (context->Id, strtod (value, NULL));
            break;
        }
        case flat::T_SINT8:
        {
            context->Record->put_sint8 (context->Id, (s_int8) string_to_sint (value, INT8_MIN, INT8_MAX));
            break;
        }
        case flat::T_SINT16:
        {
            context->Record->put_sint16 (context->Id, (s_int16) string_to_sint (value, INT16_MIN, INT16_MAX));
            break;
        }
        case flat::T_SINT32:
        {
            context->Record->put_sint32 (context->Id, (s_int32) string_to_sint (value, INT32_MIN, INT32_MAX));
            break;
        }
        case flat::T_STRING:
        {
            context->Record->put_string (context->Id, value);
            break;
        }
        case flat::T_UINT8:
        {
            context->Record->put_uint8 (context->Id, (u_int8) string_to_uint (value, UINT8_MAX));
            break;
        }
        case flat::T_UINT16:
        {
            context->Record->put_uint16 (context->Id, (u_int16) string_to_uint (value, UINT16_MAX));
            break;
        }
        case flat::T_UINT32:
        {
            context->Record->put_uint32 (context->Id, (u_int32) string_to_uint (value, UINT32_MAX));
            break;
        }
        default:
        {
            fprintf (stderr, "*** param_to_value: invalid type for value '%s'!\n", value);				
            break;
        }
    }
}

/**
 * Called when an opening tag has been processed.
 * @param ctx the parser context
 * @param name The element name
 * @param atts Element attributes
 */
static void data_start_element (void *ctx, const xmlChar *name, const xmlChar **atts)
{
    data_sax_context *context = data_sax_context::Stack.back ();

    switch (context->State)
    {
        // root node
        case disk_writer_xml::UNDEF:
        {
            // check for root node
            if (strcmp ((char*) name, XML_ROOT_NODE) == 0)
            {
                context->State = disk_writer_xml::DATA;
                
       	       	// get attribute "cs", if present
		    	if (atts != NULL) 
		    	{
		    		for (u_int32 i = 0; atts[i] != NULL; i += 2) 
		    		{
						if (atts[i][0] == 'c' && atts[i][1] == 's')
						{
							context->Id = (char*) atts[i+1];
							break;
						}		
					}
		    	}
            }
            else
            {
                fprintf (stderr, "*** data_start_element: expected <" XML_ROOT_NODE ">, but got <%s>!\n", (char*) name);
            }
            break;
        }
        // start reading list or parameter
        case disk_writer_xml::DATA:	// fall through
        case disk_writer_xml::LIST:
        {
        	// get type of element
        	flat::data_type type = flat::type_for_name ((char*) name);

        	// get attribute "id", if present
        	if (atts != NULL) 
        	{
        		for (u_int32 i = 0; atts[i] != NULL; i += 2) 
        		{
					if (atts[i][0] == 'i' && atts[i][1] == 'd')
					{
						// store checksum in the Id field
						context->Id = (char*) atts[i+1];
						break;
					}		
				}
        	}
        	
			// set type of parameter
          	context->Type = type;
            
        	// check whether we got list
        	switch (type)
        	{
        		// error
        		case flat::T_UNKNOWN:
        		{
        			return;
        		}
        		// list type
        		case flat::T_FLAT:
        		{
        			base::flat *record = new base::flat(16);
        			
        			// create child context for sublist
        			context = new data_sax_context (record);
	            	context->State = disk_writer_xml::LIST;
                    
                    break;
        		}
        		// primitive type
        		default:
        		{
                    context->Value = "";
	            	context->State = disk_writer_xml::PARAM;
        			break;
        		}
        	}
			
            break;
        }
        // error
        default:
        {
            fprintf (stderr, "*** data_start_element: entering <%s> with invalid state!\n", (char*) name);
            break;
        }
    }
}

/**
 * Called when the end of an element has been detected.
 * @param ctx the parser context
 * @param name the element name
 */
static void data_end_element (void *ctx, const xmlChar *name)
{
    data_sax_context *context = data_sax_context::Stack.back ();
    
    switch (context->State)
    {
        // finished reading parameter
        case disk_writer_xml::PARAM:
        {
        	// convert and add value of param just read 
            param_to_value (context);
            context->State = disk_writer_xml::LIST;
            break;
        }
        // finished reading list
        case disk_writer_xml::LIST:
        {
            // this could also be </Data>, in which case we do nothing
            if (data_sax_context::Stack.size() > 1)
            {
                // get parent context
                data_sax_context::Stack.pop_back();
                data_sax_context *parent = data_sax_context::Stack.back ();
                
                // add completed list to parent list
                parent->Record->put_flat (parent->Id, *(context->Record));
                
                // cleanup
                delete context->Record;
                delete context;
            }
            break;
        }
        // finished reading configuration
        case disk_writer_xml::DATA:
        {
            context->State = disk_writer_xml::UNDEF;
            break;        
        }
        // error
        default:
        {
            fprintf (stderr, "*** data_end_element: leaving </%s> with invalid state!\n", (char*) name);
            break;
        }
    }
}

/**
 * Receiving some chars from the parser.
 * @param ctx the parser context
 * @param content content of xml tag
 * @param len length of content
 */
static void data_read_characters (void *ctx, const xmlChar *content, int len)
{
    data_sax_context *context = data_sax_context::Stack.back ();

    // only read characters if we're inside a primitive type
    if (context->State == disk_writer_xml::PARAM)
    {
        // store value first and assign when closing element, as
        // 'data_read_characters' is not called for empty elements.
        context->Value = std::string ((char*) content, len);
    }
}

/**
 * Display and format error messages, gives file, line, position and
 * extra parameters.
 * @param ctx the parser context
 * @param msg error message to display
 * @param ... extra parameters for the message display
 */
static void data_parse_error (void *ctx, const char *msg, ...)
{
    va_list args;
    fprintf (stderr, "*** data_reader: ");
    va_start (args, msg);
    vfprintf (stderr, msg, args);
    va_end (args);
}

/**
 * Callbacks for the sax parser
 */
xmlSAXHandler data_sax_handler = {
    NULL, /* internalSubset */
    NULL, /* isStandalone */
    NULL, /* hasInternalSubset */
    NULL, /* hasExternalSubset */
    NULL, /* resolveEntity */
    NULL, /* getEntity */
    NULL, /* entityDecl */
    NULL, /* notationDecl */
    NULL, /* attributeDecl */
    NULL, /* elementDecl */
    NULL, /* unparsedEntityDecl */
    NULL, /* setDocumentLocator */
    NULL, /* startDocument */
    NULL, /* endDocument */
    data_start_element,
    data_end_element,
    NULL, /* reference */
    data_read_characters,
    NULL, /* ignorableWhitespace */
    NULL, /* processingInstruction */
    NULL, /* parseComment */
    NULL, /* xmlParserWarning */
    data_parse_error,
    data_parse_error,
    NULL, /* getParameterEntity */
    NULL, /* cdataBlock; */
    NULL, /* externalSubset; */
    XML_SAX2_MAGIC,
    NULL,
    NULL, /* startElementNsDebug */
    NULL, /* endElementNsDebug */
    NULL
};

// save record to XML file
bool disk_writer_xml::put_state (const std::string & name, base::flat & data) const
{
    // create empty xml document
    xmlDocPtr document = xmlNewDoc ((const xmlChar *) "1.0");
    
    // create root node
    document->children = xmlNewDocNode (document, NULL, (const xmlChar *) XML_ROOT_NODE, NULL);
    
	// recursively write content
    record_to_xml (data, document->children);

    // add checksum as attribute of root node
    std::ostringstream checksum;
    checksum << (std::hex) << data.checksum ();
    xmlSetProp (document->children, (const xmlChar *) "cs", (xmlChar*) checksum.str().c_str());
    
    // write document to file
    int retval = xmlSaveFormatFile (name.c_str (), document, 1);
    
    // cleanup
    xmlFreeDoc (document);
    
    // whether saving succeeded or not
    return retval != -1;    
}

// read record from XML file
bool disk_writer_xml::get_state (const std::string & name, base::flat & data) const
{
    // clear contents of data
    data.clear ();
    
	// prepare context
    data_sax_context ctx (&data);
    
    // read data
	if (xmlSAXUserParseFile (&data_sax_handler, &ctx, name.c_str ()) != 0)
    {
        fprintf (stderr, "*** disk_writer_xml::get_state: errors while parsing '%s'!\n", name.c_str ());
        return false;
    }
    
    // compare checksum
    std::ostringstream checksum;
    checksum << (std::hex) << data.checksum ();
    if (strcmp (checksum.str ().c_str(), ctx.Id.c_str()) != 0)
    {
        fprintf (stderr, "*** disk_writer_xml::get_state: checksum mismatch in file '%s'.\n    Data might be corrupt.\n", name.c_str());
        return false;
    }
    
    return true;    
}

// recursively convert records to XML structure
void disk_writer_xml::record_to_xml (base::flat &record, xmlNodePtr parent) const
{
	char *name;
    void *value;
    s_int32 size;
    xmlNodePtr node;
    flat::data_type type;
    const char *str_type;
    
    // reset record iterator
    record.first ();
    
    // add record data
    while ((type = record.next (&value, &size, &name)) != -1)
    {
    	str_type = flat::name_for_type (type);
    	
    	// primitive type?
    	if (type != flat::T_FLAT)
    	{
    		// convert primitive value to xml char string
    		const xmlChar* str_val = value_to_xmlChar (type, value, size);
    		node = xmlNewTextChild (parent, NULL, (const xmlChar *) str_type, str_val);
    	}
    	else
    	{
	    	// we've got a list --> recurse
            base::flat rec = base::flat ((const char *) value, size);
            node = xmlNewTextChild (parent, NULL, (const xmlChar *) str_type, NULL);
            record_to_xml (rec, node);    		
    	}
        
    	// add name attribute to node	
	    if (name != NULL && strlen (name) != 0)
        {
        	xmlSetProp (node, (const xmlChar *) "id", (const xmlChar *) name);
        }
    }
}

// convert value to xml character
xmlChar *disk_writer_xml::value_to_xmlChar (const flat::data_type & type, void *value, const u_int32 & size) const
{
	std::ostringstream tmp;
    switch (type)
    {
        // write boolean type
        case flat::T_BOOL:
        {
            tmp << (s_int32) *((u_int8*) value);
            break;
        }
        // write signed integer types
        case flat::T_SINT8:
        {
            // need extra cast, as we want to write number, not character
            tmp << (s_int32) *((s_int8*) value);
            break;
        }
        case flat::T_SINT16:
        {
            tmp << *((s_int16*) value);
            break;
        }
        case flat::T_SINT32:
        {
            tmp << *((s_int32*) value);
            break;
        }
        // write unsigned integer types
        case flat::T_UINT8:
        {
            // need extra cast, as we want to write number, not character
            tmp << (u_int32) *((u_int8*) value);
            break;
        }
        case flat::T_UINT16:
        {
            tmp << *((u_int16*) value);
            break;
        }
        case flat::T_UINT32:
        {
            tmp << *((u_int32*) value);
            break;
        }
        // write float types
        case flat::T_FLOAT:
        {
            tmp << (char *) value;
            break;
        }
        case flat::T_DOUBLE:
        {
            // FIXME
            tmp << (char *) value;
            break;
        }
        // write character
        case flat::T_CHAR:
        {
            tmp << *((char*) value);
            break;
        }
        // write string
        case flat::T_STRING:
        {
            tmp << (char*) value;
            break;
        }
        // write binary
        case flat::T_BLOB:
        {
            u_int32 j = 0;
            char *bin = (char *) value;
            char *hex = new char[(size * 2) + 1];
            hex[size * 2] = 0;
            
            for (s_int32 i = 0; i < size; i++) 
            {
                hex[j++] = Bin2Hex[(bin[i] >> 4) & 0x0f];
                hex[j++] = Bin2Hex[bin[i] & 0x0f];
            }
            
            tmp << hex;
            delete[] hex;
            
            break;
        }
    }
    
    return (xmlChar*) tmp.str().c_str();
}
