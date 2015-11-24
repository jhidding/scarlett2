#pragma once
#include <string>
#include <vector>
#include <regex.h>
#include <memory>

#include "wrap.hh"

namespace Misc
{
	class Regex;

    /*! \brief wraps the regex_t for use with the Regex class.
     */
	class Regex_buffer
	{
		typedef const regex_t * const_ptr;
		regex_t buffer;

		public:
			Regex_buffer(std::string const &expression, int flags = REG_EXTENDED);
			~Regex_buffer();
			operator const_ptr() const;
	};

    /*! \brief wraps the regmatch_t type.
     * 
     * When a string is matched to a regex, this object is filled
     * with all matches found, allowing us to iterate over all matches.
     */
	class Regex_match
	{
		friend Regex;

		typedef std::vector<regmatch_t> regvec;

		regvec match;
		std::string str;
		bool success;

		Regex_match(std::string const &s, size_t nmatch): 
			match(nmatch), str(s), success(false) {}

		regmatch_t *ref() 
			{ return match.data(); }

	public:
		typedef Wrap_fwd_iterator<std::string, regvec::const_iterator> 
			iterator;

		bool is_match() const { return success; }
		std::string operator[](size_t i) const;
		iterator begin() const;
		iterator end() const;
	};

    /*! \brief Regex wrapper class
     * 
     * Regex is a function object, returning match objects when
     * given a string.
     * The constructor takes a regular expression and nsub
     * paramater.
     */
	class Regex
	{
		std::shared_ptr<Regex_buffer> buffer;
		size_t nmatch;

		public:
			Regex() = default;

			Regex(std::string const &expression, int nsub, int flags = REG_EXTENDED):
				buffer(new Regex_buffer(expression, flags)), nmatch(nsub) {}

			Regex_match operator()(std::string const &s, int flags = 0) const;
	};
}

// vim:sw=4:ts=4:tw=80
