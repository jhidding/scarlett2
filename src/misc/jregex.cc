#include "jregex.hh"
#include "wrap.hh"

using namespace Misc;

Misc::Regex_buffer::Regex_buffer(std::string const &expression, int flags)
{
	int errc = regcomp(&buffer, expression.c_str(), flags);

	if (errc != 0)
	{
		char errstr[256];
		regerror(errc, &buffer, errstr, 255);
		throw std::string(errstr);
	}
}

Misc::Regex_buffer::~Regex_buffer()
{
	regfree(&buffer);
}

Misc::Regex_buffer::operator const_ptr() const
{
	return &buffer;
}

std::string Misc::Regex_match::operator[](size_t i) const
{
	if (match[i].rm_so != -1)
		return str.substr(match[i].rm_so,
			match[i].rm_eo - match[i].rm_so);
	else
		return "";
}

Misc::Regex_match::iterator spawn_iterator(
    std::string const &str,
	std::vector<regmatch_t>::const_iterator const &j)
{ 
	return Regex_match::iterator(j, 
		[str] (std::vector<regmatch_t>::const_iterator const &i) -> std::string
	{
		if (i->rm_so == -1) return "";
		return str.substr(i->rm_so, i->rm_eo - i->rm_so);
	});
}

Misc::Regex_match::iterator Misc::Regex_match::begin() const 
{ 
	return spawn_iterator(str, match.begin());
}

Misc::Regex_match::iterator Misc::Regex_match::end() const 
{ 
	return spawn_iterator(str, match.end());
}

Misc::Regex_match Misc::Regex::operator()(std::string const &s, int flags) const
{
	Regex_match A(s, nmatch);
	int errc = regexec(*buffer, s.c_str(), nmatch, A.ref(), flags);
	A.success = (errc == 0);

	return A;
}

