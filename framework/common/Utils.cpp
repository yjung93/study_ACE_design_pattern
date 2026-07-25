#include <sstream>
#include "Utils.hpp"

namespace Framework_Common
{

map<string, string> Utils::parseKeyValueString( const string &str )
{
    map<string, string> result;
    stringstream ss( str );
    string pair;

    while ( getline( ss, pair, '|' ) )
    {
        // Trim leading whitespace
        size_t start = pair.find_first_not_of( " \t" );
        if ( start != string::npos )
        {
            pair = pair.substr( start );
        }
        
        size_t pos = pair.find( '=' );
        if ( pos != string::npos )
        {
            string key = pair.substr( 0, pos );
            string value = pair.substr( pos + 1 );
            result[key] = value;
        }
    }
    return result;
}

string Utils::formatKeyValue( const map<string, string> &data )
{
    stringstream ss;
    bool first = true;
    for ( const auto &[key, value] : data )
    {
        if ( !first )
            ss << "| ";
        ss << key << "=" << value;
        first = false;
    }
    return ss.str();
}

}; // namespace Framework_Common