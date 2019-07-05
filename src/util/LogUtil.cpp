/*
 * Created by liyicheng on 2019/7/5.
 */

#include "LogUtil.h"

void debug(const string &message) {
    if (level <= LogLevel::DEBUG) {
        out << "[debug] " << message << '\n';
    }
}

void debug(const char* message) {
    if (level <= LogLevel::DEBUG) {
        out << "[debug] " << message << '\n';
    }
}


void debug(const function<string()> &messageSupplier) {
    if (level <= LogLevel::DEBUG) {
        debug(messageSupplier());
    }
}


void info(const string &message) {
    if (level <= LogLevel::INFO) {
        out << "[info] " << message << '\n';
    }
}


void info(const char* message) {
    if (level <= LogLevel::INFO) {
        out << "[info] " << message << '\n';
    }
}


void info(const function<string()> &messageSupplier) {
    if (level <= LogLevel::INFO) {
        info(messageSupplier());
    }
}


void warn(const string &message) {
    if (level <= LogLevel::WARN) {
        out << "[warn] " << message << '\n';
    }
}

void warn(const char* message) {
    if (level <= LogLevel::WARN) {
        out << "[warn] " << message << '\n';
    }
}

void warn(const function<string()> &messageSupplier) {
    if (level <= LogLevel::WARN) {
        warn(messageSupplier());
    }
}


void fatal(const string &message) {
    if (level <= LogLevel::FATAL) {
        out << "[fatal] " << message << '\n';
    }
}

void fatal(const char* message) {
    if (level <= LogLevel::FATAL) {
        out << "[fatal] " << message << '\n';
    }
}

void fatal(const function<string()> &messageSupplier) {
    if (level <= LogLevel::FATAL) {
        fatal(messageSupplier());
    }
}


void error(const string &message) {
    if (level <= LogLevel::ERROR) {
        out << "[error] " << message << '\n';
    }
}

void error(const char* message) {
    if (level <= LogLevel::ERROR) {
        out << "[error] " << message << '\n';
    }
}

void error(const function<string()> &messageSupplier) {
    if (level <= LogLevel::ERROR) {
        error(messageSupplier());
    }
}
