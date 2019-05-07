//
// Created by 苏畅 on 2019/5/4.
//

#include "irc_response.h"

const std::string irc::RESP_CODE::RPL_WELCOME           = "001";
const std::string irc::RESP_CODE::RPL_YOURHOST          = "002";
const std::string irc::RESP_CODE::RPL_CREATED           = "003";
const std::string irc::RESP_CODE::RPL_MYINFO            = "004";

const std::string irc::RESP_CODE::RPL_LUSERCLIENT       = "251";
const std::string irc::RESP_CODE::RPL_LUSEROP           = "252";
const std::string irc::RESP_CODE::RPL_LUSERUNKNOWN      = "253";
const std::string irc::RESP_CODE::RPL_LUSERCHANNELS     = "254";
const std::string irc::RESP_CODE::RPL_LUSERME           = "255";

const std::string irc::RESP_CODE::RPL_AWAY              = "301";
const std::string irc::RESP_CODE::RPL_UNAWAY            = "305";
const std::string irc::RESP_CODE::RPL_NOWAWAY           = "306";

const std::string irc::RESP_CODE::RPL_WHOISUSER         = "311";
const std::string irc::RESP_CODE::RPL_WHOISSERVER       = "312";
const std::string irc::RESP_CODE::RPL_WHOISOPERATOR     = "313";
const std::string irc::RESP_CODE::RPL_WHOISIDLE         = "317";
const std::string irc::RESP_CODE::RPL_ENDOFWHOIS        = "318";
const std::string irc::RESP_CODE::RPL_WHOISCHANNELS     = "319";

const std::string irc::RESP_CODE::RPL_WHOREPLY          = "352";
const std::string irc::RESP_CODE::RPL_ENDOFWHO          = "315";

const std::string irc::RESP_CODE::RPL_LIST              = "322";
const std::string irc::RESP_CODE::RPL_LISTEND           = "323";

const std::string irc::RESP_CODE::RPL_CHANNELMODEIS     = "324";

const std::string irc::RESP_CODE::RPL_NOTOPIC           = "331";
const std::string irc::RESP_CODE::RPL_TOPIC             = "332";

const std::string irc::RESP_CODE::RPL_NAMREPLY          = "353";
const std::string irc::RESP_CODE::RPL_ENDOFNAMES        = "366";

const std::string irc::RESP_CODE::RPL_MOTDSTART         = "375";
const std::string irc::RESP_CODE::RPL_MOTD              = "372";
const std::string irc::RESP_CODE::RPL_ENDOFMOTD         = "376";

const std::string irc::RESP_CODE::RPL_YOUREOPER         = "381";

const std::string irc::RESP_CODE::ERR_NOSUCHNICK        = "401";
const std::string irc::RESP_CODE::ERR_NOSUCHCHANNEL     = "403";
const std::string irc::RESP_CODE::ERR_CANNOTSENDTOCHAN  = "404";
const std::string irc::RESP_CODE::ERR_NORECIPIENT       = "411";
const std::string irc::RESP_CODE::ERR_NOTEXTTOSEND      = "412";
const std::string irc::RESP_CODE::ERR_UNKNOWNCOMMAND    = "421";
const std::string irc::RESP_CODE::ERR_NOMOTD            = "422";
const std::string irc::RESP_CODE::ERR_NONICKNAMEGIVEN   = "431";
const std::string irc::RESP_CODE::ERR_NICKNAMEINUSE     = "433";
const std::string irc::RESP_CODE::ERR_USERNOTINCHANNEL  = "441";
const std::string irc::RESP_CODE::ERR_NOTONCHANNEL      = "442";
const std::string irc::RESP_CODE::ERR_NOTREGISTERED     = "451";
const std::string irc::RESP_CODE::ERR_NEEDMOREPARAMS    = "461";
const std::string irc::RESP_CODE::ERR_ALREADYREGISTRED  = "462";
const std::string irc::RESP_CODE::ERR_PASSWDMISMATCH    = "464";
const std::string irc::RESP_CODE::ERR_UNKNOWNMODE       = "472";
const std::string irc::RESP_CODE::ERR_CHANOPRIVSNEEDED  = "482";
const std::string irc::RESP_CODE::ERR_UMODEUNKNOWNFLAG  = "501";
const std::string irc::RESP_CODE::ERR_USERSDONTMATCH    = "502";

std::string irc::IRCResponse::ToString() {
    std::string res;
    res = ":" + src + " " + code + " " + userName + " :";
    if(code == irc::RESP_CODE::RPL_WELCOME) {
        res += "Welcome to the Internet Relay Network ";
        res += cmds[0] + "!" + cmds[1] + "@" + cmds[2];
    } else if(code == irc::RESP_CODE::RPL_YOURHOST) {
        res += "Your host is " + cmds[0] + ", running version 1.0";
    } else if(code == irc::RESP_CODE::RPL_CREATED) {
        res += "This server was created 2019-5-5";
    } else if(code == irc::RESP_CODE::RPL_MYINFO) {
        res = ":" + src + " " + code + " " + userName + " ";
        res += cmds[0] + " 1.0 * *";
    } else if(code == irc::RESP_CODE::RPL_LUSERCLIENT) {
        res += "There are " + cmds[0] + " users on server and 0 services on 1 servers";
    } else if(code == irc::RESP_CODE::RPL_LUSEROP) {
        res = ":" + src + " " + code + " " + userName + " ";
        res += cmds[0] + " :operator(s) online";
    } else if(code == irc::RESP_CODE::RPL_LUSERUNKNOWN) {
        res = ":" + src + " " + code + " " + userName + " ";
        res += cmds[0] + " :unknown connection(s)";
    } else if(code == irc::RESP_CODE::RPL_LUSERCHANNELS) {
        res = ":" + src + " " + code + " " + userName + " ";
        res += cmds[0] + " :channels formed";
    } else if(code == irc::RESP_CODE::RPL_LUSERME) {
        res += "I have " + cmds[0] + " clients";
    } else if(code == irc::RESP_CODE::RPL_AWAY) {
        res += cmds[0] + " :" + cmds[1];
    } else if(code == irc::RESP_CODE::RPL_UNAWAY) {
        res += "You are no longer marked as being away";
    } else if(code == irc::RESP_CODE::RPL_NOWAWAY) {
        res += "You have been marked as being away";
    } else if(code == irc::RESP_CODE::RPL_WHOISUSER) {  
        res += cmds[0] + " " + cmds[1] + " " + cmds[2] + " :" + cmds[3];
    } else if(code == irc::RESP_CODE::RPL_WHOISSERVER) {
        res += "Server: " + src;
    } else if(code == irc::RESP_CODE::RPL_WHOISOPERATOR) {
        res += "is an IRC operator";
    } else if(code == irc::RESP_CODE::RPL_WHOISIDLE) {
        res += cmds[0] + " " + cmds[1] + ":seconds idle";
    } else if(code == irc::RESP_CODE::RPL_ENDOFWHOIS) {
        res += cmds[0] + ":End of WHOIS list"; 
    } else if(code == irc::RESP_CODE::RPL_WHOISCHANNELS) {
        res += "Not supply";
    } else if(code == irc::RESP_CODE::RPL_WHOREPLY) {
        res += "Not supply";
    } else if(code == irc::RESP_CODE::RPL_ENDOFWHO) {
        res += "Not supply";
    } else if(code == irc::RESP_CODE::RPL_LIST) {
        res += cmds[0] + " :" + cmds[1];
    } else if(code == irc::RESP_CODE::RPL_LISTEND) {
        res += "End of LIST";
    } else if(code == irc::RESP_CODE::RPL_CHANNELMODEIS) {
        res += cmds[0] + " " + cmds[1] + " " + cmds[2];
    } else if(code == irc::RESP_CODE::RPL_NOTOPIC) {
        res += cmds[0] + " :No topic is set";
    } else if(code == irc::RESP_CODE::RPL_TOPIC) {
        res += cmds[0] + " :" + cmds[1];
    } else if(code == irc::RESP_CODE::RPL_NAMREPLY) {
        res += cmds[0] + " :";
        for(int i = 1; i < cmds.size(); i++)
            res += cmds[i] + " "; 
    } else if(code == irc::RESP_CODE::RPL_ENDOFNAMES) {
        res += cmds[0] + " :End of NAMES list";
    } else if(code == irc::RESP_CODE::RPL_MOTDSTART) {
        res += "- Offline Message -";
    } else if(code == irc::RESP_CODE::RPL_MOTD) { //
        res += "- " + cmds[0];
    } else if(code == irc::RESP_CODE::RPL_ENDOFMOTD) {
        res += "End of MOTD command";
    } else if(code == irc::RESP_CODE::RPL_YOUREOPER) {
        res += "You are now an IRC operator";
    } else if(code == irc::RESP_CODE::ERR_NOSUCHNICK) {
        res += cmds[0] + " :No such nick/channel";
    } else if(code == irc::RESP_CODE::ERR_NOSUCHCHANNEL) {
        res += cmds[0] + " :No such channel";
    } else if(code == irc::RESP_CODE::ERR_CANNOTSENDTOCHAN) {
        res += cmds[0] + " :Cannot send to channel";
    } else if(code == irc::RESP_CODE::ERR_NORECIPIENT) {
        res += "No recipient given (" + cmds[0] + ")";
    } else if(code == irc::RESP_CODE::ERR_NOTEXTTOSEND) {
        res += "No text to send";
    } else if(code == irc::RESP_CODE::ERR_UNKNOWNCOMMAND) {
        res += cmds[0] + " :Unknown command";
    } else if(code == irc::RESP_CODE::ERR_NOMOTD) {
        res += "MOTD File is missing";
    } else if(code == irc::RESP_CODE::ERR_NONICKNAMEGIVEN) {
        res += "No nickname given";
    } else if(code == irc::RESP_CODE::ERR_NICKNAMEINUSE) {
        res += cmds[0] = " :Nickname is already in use";
    } else if(code == irc::RESP_CODE::ERR_USERNOTINCHANNEL) {
        res += cmds[0] + " " + cmds[1] + " :They aren't on that channel";
    } else if(code == irc::RESP_CODE::ERR_NOTONCHANNEL) {
        res += cmds[0] + " :You're not on that channel";
    } else if(code == irc::RESP_CODE::ERR_NOTREGISTERED) {
        res += "You have not registered";
    } else if(code == irc::RESP_CODE::ERR_NEEDMOREPARAMS) {
        res = ":" + src + " " + code + " " + userName + " ";
        res += cmds[0] + " :Not enough parameters";
    } else if(code == irc::RESP_CODE::ERR_ALREADYREGISTRED) {
        res += "Unauthorized command (already registered)";
    } else if(code == irc::RESP_CODE::ERR_PASSWDMISMATCH) {
        res += "Password incorrect";
    } else if(code == irc::RESP_CODE::ERR_UNKNOWNMODE) {
        res += cmds[0] + " :is unknown mode char to me for " + cmds[1];
    } else if(code == irc::RESP_CODE::ERR_CHANOPRIVSNEEDED) {
        res += cmds[0] + " :You're not channel operator";
    } else if(code == irc::RESP_CODE::ERR_UMODEUNKNOWNFLAG) {
        res += "Unknown MODE flag";
    } else if(code == irc::RESP_CODE::ERR_USERSDONTMATCH) {
        res += "Cannot change mode for other users";
    } else {
        for(auto i:cmds) {
            res += i + " ";
        }
    }
    res += "\r\n";
    return res;
}