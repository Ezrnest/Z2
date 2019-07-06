/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef HOMEWORK_LOGUTIL_H
#define HOMEWORK_LOGUTIL_H

#include <iostream>
#include <functional>
#include <string>

using namespace std;



enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    FATAL,
    ERROR
};

static ostream &out = cout;

static LogLevel level = LogLevel::INFO;

void debug(const string &message);

void debug(const function<string()> &messageSupplier);

void debug(const char *message);

void info(const string &message);

void info(const function<string()> &messageSupplier);

void info(const char *message);

void warn(const string &message);

void warn(const function<string()> &messageSupplier);

void warn(const char *message);

void fatal(const string &message);

void fatal(const function<string()> &messageSupplier);

void fatal(const char *message);

void error(const string &message);

void error(const function<string()> &messageSupplier);

void error(const char *message);

#endif //HOMEWORK_LOGUTIL_H
