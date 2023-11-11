#include "ldapmanager.h"

LDAPManager::LDAPManager()
{
    ldapUri = "ldap://ldap.technikum-wien.at:389";
    ldapVersion = LDAP_VERSION3;

    // setup LDAP connection
    if(ldap_initialize(&ldapHandle, &ldapUri[0]) != LDAP_SUCCESS)
        throw std::runtime_error("ldap_init failed");

    // set LDAP version
    if(ldap_set_option(ldapHandle, LDAP_OPT_PROTOCOL_VERSION, &ldapVersion) != LDAP_SUCCESS)
        throw std::runtime_error("failed to set ldap version");

    // initialize tls
    if(ldap_start_tls_s(ldapHandle, nullptr, nullptr) != LDAP_SUCCESS)
        throw std::runtime_error("failed to start tls (ldap)");
}

// public functions

bool LDAPManager::authenticate(std::string username, std::string password)
{
    // bind to ldap server
    std::string toSearch = "uid=" + username + ",ou=people,dc=technikum-wien,dc=at";

    BerValue* serverCredentials;
    BerValue bindCredentials;

    bindCredentials.bv_val = (char*)password.c_str();
    bindCredentials.bv_len = password.size();

    if(ldap_sasl_bind_s(ldapHandle, toSearch.c_str(), LDAP_SASL_SIMPLE,
                         &bindCredentials, nullptr, nullptr, &serverCredentials) != LDAP_SUCCESS)
        return false;

    return true;
}




















