#include <map>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>

namespace nana
{


/** A grid, based on the listbox */
class grid  : public listbox
{
public:
    /** Constructor
        @param[in] wd parent window
        @param[in] r  grid size on screen pixels
    */
    grid( window wd, const rectangle& r = rectangle());

    /** Resize the grid */
    void Resize( int rows, int cols );

    void ColTitle( int col, const std::string& value );

    /** Set cell value */
    void Set( int row, int col, const std::string& value );

protected:
    int myRowCount;
    int myColCount;

    /** true if row and col are included */
    bool CheckIndex( int row, int col );


};
template <class T>
class property
{
public:
    std::string myName;
    T myValue;

    property( const std::string& name,
             const T& value )
             {
                 myName = name;
                 myValue = value;
             }
    std::string ValueAsString() const
    {
        std::stringstream ss;
        ss.str("");
        ss << myValue;
        return ss.str();
    }
};

/** Property grid for handling name/value pairs */
class property_grid : public grid
{
public:
    property_grid( window wd, const rectangle& r = rectangle());

    /** Add a new property */
    void Add(
        const std::string& name,
        const std::string& value );

    template <class T>
    void Add( const property<T>& prop )
    {
        Add( prop.myName, prop.ValueAsString() );
    }


    /** Change value of existing property */
    void Set(
        const std::string& name,
        const std::string& value );

    template <class T>
    void Set( const property<T>& prop )
    {
        Set( prop.myName, prop.ValueAsString() );
    }

private:
    int myPropCount;
    std::map< std::string, int > myMap;

};
}
