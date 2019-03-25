/**
 * Copyright (c) 2018, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 * Boost Software License - Version 1.0
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
*
*/
#pragma once

#include <functional>
#include <vector>
#include <hadoken/string/string_view.hpp>

namespace hadoken{


class option;
class sub_command;
class options_handler;

///
/// \brief parameter subcommand
///
class options_handler{
public:

    void add_option(option opt);

    void add_subcommand(sub_command sub_com);

    const std::vector<sub_command> & sub_commands() const;

    const std::vector<option> & options() const;
private:
    std::vector<option> _opts;
    std::vector<sub_command> _subcmd;

};


///
/// \brief parameter subcommand
///
class sub_command : public options_handler{
public:
    sub_command(string_view subcommand_name, std::function<void (void)> & callback, string_view help_msg);

    void add_option(option opt);

    string_view name() const;

    // internal
    void _call() const;
private:
    string_view _name, _help_msg;

    std::vector<option> _options;
    std::function<void (void)> _action;
};


///
/// \brief a given option
///
class option{
public:

    option(string_view option_name, string_view help_msg,  std::function<void (std::string)> callback);
    option(string_view option_name,  string_view help_msg, std::function<void (int)> callback);

    string_view name() const;
private:
    string_view _name, _help_msg;

    std::function<void (string_view opt)> _action;
};




void parse_options(const options_handler & opt_handler, int argc, char** argv);

void parse_options(const options_handler & opt_handler, string_view prog_name, const std::vector<string_view> & options);

} // hadoken


#include "impl/getopts_impl.hpp"

