/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2014 David Quintana [gigaherz]
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */


#include <cstdlib>

//#include <winsock2.h>
#include "../DEV9.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>

#include <libxml/parser.h>
#include <libxml/tree.h>

void SaveConf() {

    xmlDocPtr doc = nullptr;       /* document pointer */
    xmlNodePtr root_node = nullptr;
    char buff[256];

    /*
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(nullptr, BAD_CAST "dev9");
    xmlDocSetRootElement(doc, root_node);

    xmlNewChild(root_node, nullptr, BAD_CAST "Eth",
        BAD_CAST config.Eth);

    xmlNewChild(root_node, nullptr, BAD_CAST "Hdd",
        BAD_CAST config.Hdd);

    sprintf(buff,"%d",config.HddSize);
    xmlNewChild(root_node, nullptr, BAD_CAST "HddSize",
        BAD_CAST buff);

    sprintf(buff,"%d",config.ethEnable);
    xmlNewChild(root_node, nullptr, BAD_CAST "ethEnable",
        BAD_CAST buff);

    sprintf(buff,"%d",config.hddEnable);
    xmlNewChild(root_node, nullptr, BAD_CAST "hddEnable",
        BAD_CAST buff);
    /*
     * Dumping document to stdio or file
     */


    const std::string file(s_strIniPath + "dev9ghzdrk.cfg");

    xmlSaveFormatFileEnc(file.c_str(), doc, "UTF-8", 1);
//    free(configFile);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
}

void LoadConf() {

    const std::string file(s_strIniPath + "dev9ghzdrk.cfg");
    if( -1 == access( file.c_str(), F_OK ) )
        return;

    memset(&config, 0, sizeof(config));

    // Read the files
    xmlDoc *doc = nullptr;
    xmlNode *cur_node = nullptr;

    doc = xmlReadFile(file.c_str(), nullptr, 0);

    if (doc == nullptr){
        SysMessage("Unable to parse configuration file! Suggest deleting it and starting over.");
    }

    for (cur_node = xmlDocGetRootElement(doc)->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //            printf("node type: Element, name: %s\n", cur_node->name);
            if(0 == strcmp((const char*)cur_node->name, "Eth")) {
            strcpy(config.Eth, (const char*)xmlNodeGetContent(cur_node));
            }
            if(0 == strcmp((const char*)cur_node->name, "Hdd")) {
            strcpy(config.Hdd, (const char*)xmlNodeGetContent(cur_node));
            }
            if(0 == strcmp((const char*)cur_node->name, "HddSize")) {
            config.HddSize = atoi((const char*)xmlNodeGetContent(cur_node));
            }
            if(0 == strcmp((const char*)cur_node->name, "ethEnable")) {
            config.ethEnable = atoi((const char*)xmlNodeGetContent(cur_node));
            }
            if(0 == strcmp((const char*)cur_node->name, "hddEnable")) {
            config.hddEnable = atoi((const char*)xmlNodeGetContent(cur_node));
            }
        }
    }

//    free(configFile);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}
