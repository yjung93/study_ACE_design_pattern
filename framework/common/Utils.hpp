#ifndef FRAMEWORK_COMMON_UTILS_HPP_
#define FRAMEWORK_COMMON_UTILS_HPP_

#include <map>
#include <string>

using namespace std;

namespace Framework_Common
{
class Utils
{
  public:
    Utils() = default;
    ~Utils() = default;

    static string formatKeyValue( const map<string, string> &data );
    static map<string, string> parseKeyValueString( const string &str );

  private:
};
} // namespace Framework_Common

#endif //FRAMEWORK_COMMON_UTILS_HPP_
