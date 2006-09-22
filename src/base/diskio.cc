/*
   $Id: diskio.cc,v 1.5 2006/09/22 01:15:22 ksterker Exp $

   Copyright (C) 2004/2006 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file base/diskio.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief More secure I/O classes for writing to/reading from file.
 */

#include <sstream>
#include <libxml/parser.h>

#include "base/diskio.h"

using base::flat;
using base::diskio;

// for converting binary to ascii
char *diskio::Bin2Hex = "0123456789ABCDEF";

// Name of xml data file root node
#define XML_ROOT_NODE "Data"

/**
 * Context for the sax parser
 */
struct data_sax_context 
{
    /**
     * create a new parser context
     * @param config the configuration to be loaded
     */
    data_sax_context (base::flat &record, data_sax_context* parent = NULL)
    {
        Record = &record;
        State = diskio::UNDEF;
        ParentCtx = parent;
    }
    
    /**
     * Return parent context.
     */
    data_sax_context *pop ()
    {
    	return ParentCtx;
    }
    
    /// Storage for parameters
    base::flat *Record;
    /// id attribute of current element
    std::string Id;
    /// type of current element
    flat::data_type Type;
    /// current state of sax parser
    u_int8 State;
    /// parent of this context
    data_sax_context *ParentCtx;
};

/**
 * Called when an opening tag has been processed.
 * @param ctx the parser context
 * @param name The element name
 * @param atts Element attributes
 */
static void data_start_element (void *ctx, const xmlChar *name, const xmlChar **atts)
{
    data_sax_context *context = (data_sax_context*) ctx;

    switch (context->State)
    {
        // root node
        case diskio::UNDEF:
        {
            // check for root node
            if (strcmp ((char*) name, XML_ROOT_NODE) == 0)
            {
                context->State = diskio::DATA;
            }
            else
            {
                fprintf (stderr, "*** data_start_element: expected <" XML_ROOT_NODE ">, but got <%s>!\n", (char*) name);
            }
            break;
        }
        // start reading list or parameter
        case diskio::DATA:	// fall through
        case diskio::LIST:
        {
        	// get type of element
        	flat::data_type type = flat::type_for_name ((char*) name);
        	
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
        			base::flat record(16);
        			context = new data_sax_context (record, context);
	            	context->State = diskio::LIST;
	            	
	            	ctx = context;
	            	break;
        		}
        		// primitive type
        		default:
        		{
	            	context->State = diskio::PARAM;
        			break;
        		}
        	}
        	
        	// get attribute "id", if present
        	if (atts != NULL) 
        	{
        		for (u_int32 i = 0; atts[i] != NULL; i += 2) 
        		{
					if (atts[i][0] == 'i' && atts[i][1] == 'd')
					{
						context->Id = (char*) atts[i+1];
					}		
				}
        	}

			// set type of character
          	context->Type = type;
			
            break;
        }
        // error
        default:
        {
            fprintf (stderr, "*** data_start_element: invalid state when reading '%s'!\n", (char*) name);
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
    data_sax_context *context = (data_sax_context*) ctx;
    
    switch (context->State)
    {
        // finished reading parameter
        case diskio::PARAM:
        {
            context->State = diskio::LIST;
            break;
        }
        // finished reading list
        case diskio::LIST:
        {
        	// TODO: add flat
        	
            ctx = context->pop ();
            delete context;
            break;
        }
        // finished reading configuration
        case diskio::DATA:
        {
            context->State = diskio::UNDEF;
            break;        
        }
        // error
        default:
        {
            fprintf (stderr, "*** data_end_element: invalid state!\n");
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
    data_sax_context *context = (data_sax_context*) ctx;

    // only read characters if we're inside a primitive type
    if (context->State == diskio::PARAM)
    {
        string value ((char*) content, len);
        
		// TODO: add parameter
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

// ctor
diskio::diskio () : flat (256)
{
}

// read record from files
bool diskio::get_record (igzstream & in)
{
    if (in.eof ()) {
        fprintf (stderr, "*** diskio::get_record: already at end of file\n");
        return false;
    }
    
    u_int32 length, checksum;
    length << in;
    
    char *buffer = new char[length];
    if (!buffer) {
        fprintf (stderr, "*** diskio::get_record: failed to allocate %i bytes. Giving up ...\n", length);
        return false;
    }
    
    in.get_block (buffer, length);
    setBuffer (buffer, length);
    
    checksum << in;
    if (checksum != this->checksum ()) {
        fprintf (stderr, "*** diskio::get_record: checksum error. Data might be corrupt.\n");
        return false;
    }
    
    return true;
}

// write record to file
void diskio::put_record (ogzstream & out)
{
    size () >> out;
    out.put_block ((void*) getBuffer (), size ());
    checksum () >> out;
    
     // reset
     clear (); 
}

// read record from XML file
bool diskio::get_xml (const std::string & filename)
{
    data_sax_context ctx (*this);
    
	if (xmlSAXUserParseFile (&data_sax_handler, &ctx, filename.c_str ()) != 0)
    {
        fprintf (stderr, "*** diskio::get_xml: errors while parsing '%s'!\n", filename.c_str ());
        return false;
    }
    
    return true;
}

// save record to XML file
bool diskio::put_xml (const std::string & filename)
{
    // create empty xml document
    xmlDocPtr document = xmlNewDoc ((const xmlChar *) "1.0");

    // create root node
    document->children = xmlNewDocNode (document, NULL, (const xmlChar *) XML_ROOT_NODE, NULL);

	// recursively write content
    record_to_xml (*this, document->children);
    
    // write document to file
    int retval = xmlSaveFormatFile (filename.c_str (), document, 1);

    // cleanup
    xmlFreeDoc (document);
    
    // whether saving succeeded or not
    return retval != -1;
}

// recursively convert records to XML structure
void diskio::record_to_xml (base::flat &record, xmlNodePtr parent) const
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
xmlChar *diskio::value_to_xmlChar (const flat::data_type & type, void *value, const u_int32 & size) const
{
	std::ostringstream tmp;
    switch (type)
    {
        // write boolean type
        case flat::T_BOOL:
        {
            tmp << *((bool*) value);
            break;
        }
        // write signed integer types
        case flat::T_SINT8:
        {
            tmp <<  *((s_int8*) value);
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
            tmp << *((u_int8*) value);
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
