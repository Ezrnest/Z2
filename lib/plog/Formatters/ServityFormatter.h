//
// Created by liyicheng on 2019/7/26.
//

#ifndef Z2_SERVITYFORMATTER_H
#define Z2_SERVITYFORMATTER_H

#include <plog/Util.h>
#include <plog/Record.h>
#include <iomanip>
namespace plog
{
class ServityFormatter
{
public:
    static util::nstring header()
    {
        return util::nstring();
    }

    static util::nstring format(const Record& record)
    {
        util::nostringstream ss;
        ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR(" ");
//        ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("] ");
//        ss << PLOG_NSTR("[") << record.getFunc() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR("] ");
        ss << record.getMessage() << PLOG_NSTR("\n");

        return ss.str();
    }
};
}
#endif //Z2_SERVITYFORMATTER_H
