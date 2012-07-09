/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
Name: log_commandscript
%Complete: 100
Comment: All logs related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "ObjectMgr.h"

class log_commandscript : public CommandScript
{
public:
    log_commandscript() : CommandScript("log_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand logCommandTable[] =
        {
            { "spam",           SEC_GAMEMASTER,     true,  &HandleLogSpamCommand,           "", NULL },
            { NULL,             0,                  false, NULL,                            "", NULL }
        };
        static ChatCommand commandTable[] =
        {
            { "log",          SEC_GAMEMASTER,       true, NULL,                    "", logCommandTable },
            { NULL,             0,                  false, NULL,                                "", NULL }
        };
        return commandTable;
    }
    
    static bool HandleLogSpamCommand(ChatHandler* handler, char const* args)
    {
        char* nameStr = strtok((char*)args, "");
        if (!nameStr)
            return false;

        Player* target;
        uint64 target_guid;
        std::string target_name;
        if (!handler->extractPlayerTarget(nameStr, &target, &target_guid, &target_name))
            return false;

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_SPAM_REPORTS_BY_GUID);
        stmt->setUInt64(0, target_guid);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            do
            {
                Field* fields = result->Fetch();
                uint32 reporterGuid = fields[0].GetUInt32();
                uint8 type = fields[1].GetUInt8();
                uint8 languaje = fields[2].GetUInt8();
                uint32 mailOrType = fields[3].GetUInt32();
                std::string description = fields[4].GetString();

                std::string reporterName;
                sObjectMgr->GetPlayerNameByGUID(reporterGuid, reporterName);

                if (type)
                {
                    std::string languajeStr, chatTypeStr;
                    switch (languaje)
                    {
                        case LANG_UNIVERSAL:
                            languajeStr = "Universal";
                            break;
                        case LANG_ORCISH:
                            languajeStr = "Orcish";
                            break;
                        case LANG_DARNASSIAN:
                            languajeStr = "Darnassian";
                            break;
                        case LANG_TAURAHE:
                            languajeStr = "Taurahe";
                            break;
                        case LANG_DWARVISH:
                            languajeStr = "Dwarvish";
                            break;
                        case LANG_COMMON:
                            languajeStr = "Common";
                            break;
                        case LANG_DEMONIC:
                            languajeStr = "Demonic";
                            break;
                        case LANG_TITAN:
                            languajeStr = "Titan";
                            break;
                        case LANG_DRACONIC:
                            languajeStr = "Draconic";
                            break;
                        case LANG_KALIMAG:
                            languajeStr = "Kalimag";
                            break;
                        case LANG_GNOMISH:
                            languajeStr = "Gnomish";
                            break;
                        case LANG_TROLL:
                            languajeStr = "Troll";
                            break;
                        case LANG_GUTTERSPEAK:
                            languajeStr = "Gutterspeak";
                            break;
                        case LANG_DRAENEI:
                            languajeStr = "Draenei";
                            break;
                        case LANG_ZOMBIE:
                            languajeStr = "Zombie";
                            break;
                        case LANG_GNOMISH_BINARY:
                            languajeStr = "Gnomish binary";
                            break;
                        case LANG_GOBLIN_BINARY:
                            languajeStr = "Goblin binary";
                            break;
                        default: 
                            languajeStr = "Unknown";
                            break;
                    }
                    switch (mailOrType)
                    {
                        case CHAT_MSG_SAY:                     // 0x01,
                            chatTypeStr = "Say";
                            break;
                        case CHAT_MSG_PARTY:                   // 0x02,
                            chatTypeStr = "Party";
                            break;
                        case CHAT_MSG_RAID:                    // 0x03,
                            chatTypeStr = "Raid";
                            break;
                        case CHAT_MSG_GUILD:                   // 0x04,
                            chatTypeStr = "Guild";
                            break;
                        case CHAT_MSG_OFFICER:                 // 0x05,
                            chatTypeStr = "Guild Officer";
                            break;
                        case CHAT_MSG_YELL:                    // 0x06,
                            chatTypeStr = "Yell";
                            break;
                        case CHAT_MSG_WHISPER:                 // 0x07,
                            chatTypeStr = "Whisper";
                            break;
                        case CHAT_MSG_CHANNEL:                 // 0x11,
                            chatTypeStr = "Channel";
                            break;
                        case CHAT_MSG_RAID_LEADER:             // 0x27,
                            chatTypeStr = "Raid leader";
                            break;
                        case CHAT_MSG_RAID_WARNING:            // 0x28,
                            chatTypeStr = "Raid warning";
                            break;
                        case CHAT_MSG_BATTLEGROUND:            // 0x2C,
                            chatTypeStr = "Battleground";
                            break;
                        case CHAT_MSG_BATTLEGROUND_LEADER:     // 0x2D,
                            chatTypeStr = "BG Leader";
                            break;
                        case CHAT_MSG_PARTY_LEADER:            // 0x33
                            chatTypeStr = "Party leader";
                            break;
                        default:
                            chatTypeStr = "Unknown";
                            break;
                    }
                    handler->PSendSysMessage(LANG_REPORT_SPAM_CHAT, reporterName.c_str(), reporterGuid, languajeStr.c_str(), chatTypeStr.c_str(), description.c_str());
                }
                else
                    handler->PSendSysMessage(LANG_REPORT_SPAM_MAIL, reporterName.c_str(), reporterGuid, mailOrType, description.c_str());
            }
            while (result->NextRow());
        }
        return true;
    }

    static bool HandleLogIpCommand(ChatHandler* handler, char const* args)
    {
        return true;
    }

};

void AddSC_log_commandscript()
{
    new log_commandscript();
}
