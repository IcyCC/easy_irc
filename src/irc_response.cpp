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
    res = res + src + " ";
    res = res + code + " ";
    for (auto &i: cmds ){
        res = res + i + " ";
    }
    res  = res + "\r\n";
    return res;
}

std::string irc::IRCUserResponse::ToString() {
    std::string res;
    if(code == irc::RESP_CODE::RPL_WELCOME) {
        res = "Welcome to the Internet Replay Network ";
        res += sender->nickName + "!" + sender->userName + "@" + sender->ircHost + "\r\n";
    } else if(code == irc::RESP_CODE::RPL_YOURHOST) {
        res = "Your host is " + sender->ircHost + ", running version 1\r\n";
    } else if(code == irc::RESP_CODE::RPL_CREATED) {
        res = "This server was created 2019-5-5\r\n";
    } else if(code == irc::RESP_CODE::RPL_MYINFO) {
        res = sender->ircHost + "\r\n";
    } else if(code == irc::RESP_CODE::RPL_LUSERCLIENT) {
        res = ":There are " + cmds[0] + " users on server\r\n";
    } else if(code == irc::RESP_CODE::RPL_LUSEROP) {
        res = cmds[0] + " :operator(s) online\r\n";
    } else if(code == irc::RESP_CODE::RPL_LUSERUNKNOWN) {
        res = cmds[0] + " :unknown connection(s)\r\n";
    } else if(code == irc::RESP_CODE::RPL_LUSERCHANNELS) {
        res = cmds[0] + " :channels formed\r\n";
    } else if(code == irc::RESP_CODE::RPL_LUSERME) {
        res = ":I have " + cmds[0] + " clients\r\n";
    } else if(code == irc::RESP_CODE::RPL_AWAY) {
        res = sender->nickName + " :" + cmds[1] + "\r\n";
    } else if(code == irc::RESP_CODE::RPL_UNAWAY) {
        res = ":You are no longer marked as being away\r\n";
    } else if(code == irc::RESP_CODE::RPL_NOWAWAY) {
        res = ":You have been marked as being away\r\n";
    } else if(code == irc::RESP_CODE::RPL_WHOISUSER) {  
        res = cmds[0] + " " + cmds[1] + " " + sender->ircHost + " * :" + sender->userName + "\r\n";
    } else if(code == irc::RESP_CODE::RPL_WHOISSERVER) {
        res = "Server: " + src + "\r\n";
    } else if(code == irc::RESP_CODE::RPL_WHOISOPERATOR) {
        res = "Admin :is an IRC operator\r\n";
    } else if(code == irc::RESP_CODE::RPL_WHOISIDLE) {
        res = cmds[0] + " " + cmds[1] + ":seconds idle\r\n";
    } else if(code == irc::RESP_CODE::RPL_ENDOFWHOIS) {
        res = cmds[0] + ":End of WHOIS list\r\n"; 
    } else if(code == irc::RESP_CODE::RPL_WHOISCHANNELS) {
        res = "Not supply\r\n";
    } else if(code == irc::RESP_CODE::RPL_WHOREPLY) {
        
    } else if(code == irc::RESP_CODE::RPL_ENDOFWHO) {

    } else if(code == irc::RESP_CODE::RPL_LIST) {

    } else if(code == irc::RESP_CODE::RPL_LISTEND) {

    } else if(code == irc::RESP_CODE::RPL_CHANNELMODEIS) {

    } else if(code == irc::RESP_CODE::RPL_NOTOPIC) {

    } else if(code == irc::RESP_CODE::RPL_TOPIC) {

    } else if(code == irc::RESP_CODE::RPL_NAMREPLY) {

    } else if(code == irc::RESP_CODE::RPL_ENDOFNAMES) {

    } else if(code == irc::RESP_CODE::RPL_MOTDSTART) {

    } else if(code == irc::RESP_CODE::RPL_MOTD) {

    } else if(code == irc::RESP_CODE::RPL_ENDOFMOTD) {

    } else if(code == irc::RESP_CODE::RPL_YOUREOPER) {

    } else if(code == irc::RESP_CODE::ERR_NOSUCHNICK) {

    } else if(code == irc::RESP_CODE::ERR_NOSUCHCHANNEL) {
        
    } else if(code == irc::RESP_CODE::ERR_CANNOTSENDTOCHAN) {

    } else if(code == irc::RESP_CODE::ERR_NORECIPIENT) {

    } else if(code == irc::RESP_CODE::ERR_NOTEXTTOSEND) {

    } else if(code == irc::RESP_CODE::ERR_UNKNOWNCOMMAND) {

    } else if(code == irc::RESP_CODE::ERR_NOMOTD) {

    } else if(code == irc::RESP_CODE::ERR_NONICKNAMEGIVEN) {

    } else if(code == irc::RESP_CODE::ERR_NICKNAMEINUSE) {

    } else if(code == irc::RESP_CODE::ERR_USERNOTINCHANNEL) {

    } else if(code == irc::RESP_CODE::ERR_NOTONCHANNEL) {

    } else if(code == irc::RESP_CODE::ERR_NOTREGISTERED) {

    } else if(code == irc::RESP_CODE::ERR_NEEDMOREPARAMS) {

    } else if(code == irc::RESP_CODE::ERR_ALREADYREGISTRED) {

    } else if(code == irc::RESP_CODE::ERR_PASSWDMISMATCH) {

    } else if(code == irc::RESP_CODE::ERR_UNKNOWNMODE) {

    } else if(code == irc::RESP_CODE::ERR_CHANOPRIVSNEEDED) {

    } else if(code == irc::RESP_CODE::ERR_UMODEUNKNOWNFLAG) {

    } else if(code == irc::RESP_CODE::ERR_USERSDONTMATCH) {

    } else {

    }

    return res;
}