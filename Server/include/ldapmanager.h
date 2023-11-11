#ifndef LDAPMANAGER_H
#define LDAPMANAGER_H

#include <ldap.h>
#include <string>
#include <exception>
#include <stdexcept>
#include <iostream>

class LDAPManager
{
public:
    //ctor
    LDAPManager();

    // public functions
    bool authenticate(std::string username, std::string password);

private:
    // private variables
    LDAP *ldapHandle;

    std::string ldapUri;
    int ldapVersion;
};

#endif // LDAPMANAGER_H
