#include <boost/uuid/uuid.hpp>

namespace TechDraw {
class Tag {
public:
    //Uniqueness
    boost::uuids::uuid getTag() const;
    virtual std::string getTagAsString() const;

protected:
    void createNewTag();
    boost::uuids::uuid tag;
};
}