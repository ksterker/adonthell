/*
   $Id: configio.cc,v 1.5 2008/03/18 21:46:22 ksterker Exp $

   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
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
 * @file 	base/configio.cc
 * @author 	Kai Sterker <kaisterker@linuxgames.com>
 *
 * @brief 	Loads and saves configuration file.
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "configio.h"
#include "logging.h"

using base::config_io;
using base::cfg_option;
using base::configuration;

/**
 * Context for the sax parser
 */
struct cfg_sax_context 
{
    /**
     * create a new parser context
     * @param config the configuration to be loaded
     */
    cfg_sax_context (configuration *config)
    {
        Config = config;
        State = config_io::UNDEF;
    }
    
    /// Storage for configuration settings
    configuration *Config;
    /// name of current section
    string Section;
    /// last option read
    string Option;
    /// last comment read
    string Comment;
    /// Current state
    u_int8 State;
};
    

/**
 * Parse a comment.
 * @param ctx the parser context
 * @param value the comment content
 */
static void cfg_parse_comment (void *ctx, const xmlChar *value)
{
    cfg_sax_context *context = (cfg_sax_context*) ctx;
    
    // only store comments when we're inside a section
    if (context->State != config_io::SECTION) return;
    else context->Comment = (char*) value;
}

/**
 * Called when an opening tag has been processed.
 * @param ctx the parser context
 * @param name The element name
 * @param atts Element attributes
 */
static void cfg_start_element (void *ctx, const xmlChar *name, const xmlChar **atts)
{
    cfg_sax_context *context = (cfg_sax_context*) ctx;

    switch (context->State)
    {
        // root node
        case config_io::UNDEF:
        {
            // check for root node
            if (strcmp ((char*) name, "Configuration") == 0)
                context->State = config_io::START;        
            else
                LOG(ERROR) << "cfg_start_element: expected <Configuration>, but got <"
                           << (char*) name << ">";
            break;
        }
        // start reading section
        case config_io::START:
        {
            context->State = config_io::SECTION;
            context->Section = (char *) name;
            context->Comment = "";
            break;
        }
        // start reading option
        case config_io::SECTION:
        {
            context->State = config_io::OPTION;
            context->Option = (char *) name;
            break;
        }
        // error
        default:
        {
            LOG(ERROR) << "cfg_start_element: invalid state!";
            break;
        }
    }
}

/**
 * Called when the end of an element has been detected.
 * @param ctx the parser context
 * @param name the element name
 */
static void cfg_end_element (void *ctx, const xmlChar *name)
{
    cfg_sax_context *context = (cfg_sax_context*) ctx;
    
    switch (context->State)
    {
        // finished reading option
        case config_io::OPTION:
        {
            context->State = config_io::SECTION;
            context->Comment = "";
            break;
        }
        // finished reading section
        case config_io::SECTION:
        {
            context->State = config_io::START;
            context->Comment = "";
            break;
        }
        case config_io::START:
        {
            context->State = config_io::UNDEF;
            break;        
        }
        // error
        default:
        {
            LOG(ERROR) << "cfg_end_element: invalid state!";
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
static void cfg_read_characters (void *ctx, const xmlChar *content, int len)
{
    cfg_sax_context *context = (cfg_sax_context*) ctx;

    // only read characters if we're inside OPTION
    if (context->State == config_io::OPTION)
    {
        string value ((char*) content, len);
        
        cfg_option *option = new cfg_option (value);
        option->set_comment (context->Comment);
        
        context->Config->add_option (context->Section, context->Option, option);
    }
}

/**
 * Display and format error messages, gives file, line, position and
 * extra parameters.
 * @param ctx the parser context
 * @param msg error message to display
 * @param ... extra parameters for the message display
 */
static void cfg_parse_error (void *ctx, const char *msg, ...)
{
    va_list args;
    va_start (args, msg);
    char err_msg[1024];
    vsprintf (err_msg, msg, args);  /// will this work on a C++ string?
    va_end (args);

    LOG(ERROR) << "config_reader: " << err_msg;
}

/**
 * Callbacks for the sax parser
 */
xmlSAXHandler cfg_sax_handler = {
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
    cfg_start_element,
    cfg_end_element,
    NULL, /* reference */
    cfg_read_characters,
    NULL, /* ignorableWhitespace */
    NULL, /* processingInstruction */
    cfg_parse_comment,
    NULL, /* xmlParserWarning */
    cfg_parse_error,
    cfg_parse_error,
    NULL, /* getParameterEntity */
    NULL, /* cdataBlock; */
    NULL, /* externalSubset; */
    XML_SAX2_MAGIC,
    NULL,
    NULL, /* startElementNsDebug */
    NULL, /* endElementNsDebug */
    NULL
};


// load config file from disk
bool config_io::read (const std::string & filename, configuration *config)
{
    cfg_sax_context ctx (config);

    LOG(INFO) << "reading " << filename;
    
	if (xmlSAXUserParseFile (&cfg_sax_handler, &ctx, filename.c_str ()) != 0)
    {
        LOG(ERROR) << "config_io::read: errors while parsing '" << filename.c_str () << "'";
        return false;
    }
    
    return true;
}

// save config file to disk
bool config_io::write (const std::string & filename, const configuration *config)
{
    // create empty xml document
    xmlDocPtr document = xmlNewDoc ((const xmlChar *) "1.0");

    // create root node
    document->children = xmlNewDocNode (document, NULL, (const xmlChar *) "Configuration", NULL);
    
    xmlNodePtr section;
    vector<const char*> options;
    
    // get all sections in configuration
    vector<const char*> sections = config->get_sections (true);
    for (vector<const char*>::const_iterator i = sections.begin (); i != sections.end (); i++)
    {
        section = xmlNewTextChild (document->children, NULL, (const xmlChar *) *i, NULL);
        
        // get all options of section
        options = config->get_options (*i, true);
        for (vector<const char*>::const_iterator o = options.begin (); o != options.end (); o++)
        {
            cfg_option *opt = config->option (*i, *o, cfg_option::UNDEF);
            if (opt->comment () != "") 
            {
                xmlNodePtr comment = xmlNewComment ((const xmlChar *) opt->comment ().c_str ());
                xmlAddChild (section, comment);
            }
            xmlNewTextChild (section, NULL, (const xmlChar *) *o, (const xmlChar *) opt->value ().c_str ());
        }
    }
    
    // write document to file
    int retval = xmlSaveFormatFile (filename.c_str (), document, 1);

    // cleanup
    xmlFreeDoc (document);
    
    // whether saving succeeded or not
    return retval != -1;
}
