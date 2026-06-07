#include <iostream>
#include <string>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>

#include "json.h"
#include "sort.h"
#include "net.h"
#include "devcore.h"
#include "lib/Log.h"

namespace logging = boost::log;

static void initLog()
{
    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::info);
    logging::add_console_log(
        std::cout, logging::keywords::format = "[%TimeStamp%] %Severity% %Message%");
}

int main(int argc, char* argv[]) {
    initLog();
    LOG(INFO) << LOG_BADGE("main") << LOG_DESC("learn_boost started");

    std::string cmd = (argc > 1) ? argv[1] : "exception";

    if (cmd == "sort") {
        use_sort();
    } else if (cmd == "net") {
        use_network();
    } else if (cmd == "json") {
        use_json();
    } else if (cmd == "base64") {
        use_base64();
    } else if (cmd == "uuid") {
        use_uuid();
    } else if (cmd == "aes") {
        use_aes();
    } else if (cmd == "hash") {
        use_hash();
    } else if (cmd == "ini") {
        use_ini();
    } else if (cmd == "log") {
        use_log();
    } else if (cmd == "exception") {
        use_exception();
    } else {
        LOG(WARNING) << LOG_BADGE("main") << LOG_DESC("unknown command")
                     << LOG_KV("cmd", cmd);
    }

    LOG(INFO) << LOG_BADGE("main") << LOG_DESC("learn_boost finished");
    return 0;
}