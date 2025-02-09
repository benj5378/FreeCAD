#include "PreCompiled.h"
#ifndef _PreComp_
    #include <boost/random.hpp>
    #include <boost/uuid/uuid_io.hpp>
    #include <boost/uuid/uuid_generators.hpp>
#endif


boost::uuids::uuid Tag::getTag() const
{
    return tag;
}

std::string Tag::getTagAsString() const
{
    return boost::uuids::to_string(getTag());
}

void Tag::createNewTag()
{
    // Initialize a random number generator, to avoid Valgrind false positives.
    // The random number generator is not threadsafe so we guard it.  See
    // https://www.boost.org/doc/libs/1_62_0/libs/uuid/uuid.html#Design%20notes
    static boost::mt19937 ran;
    static bool seeded = false;
    static boost::mutex random_number_mutex;

    boost::lock_guard<boost::mutex> guard(random_number_mutex);

    if (!seeded) {
        ran.seed(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
    static boost::uuids::basic_random_generator<boost::mt19937> gen(&ran);


    tag = gen();
}