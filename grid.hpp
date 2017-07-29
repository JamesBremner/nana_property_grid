#pragma once
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

    /** Title the column
        @param[in] col zero-based column index
        @param[in] value the column title
    */
    void ColTitle( int col, const std::string& value );

    /** Specify column width
     @param[in] col zero-based column index
     @param[in] width in pixels
    */
    void ColWidth( int col, int width );

    /** Set cell value */
    void Set( int row, int col, const std::string& value );

protected:
    int myRowCount;
    int myColCount;

    /** true if row and col are included */
    bool CheckIndex( int row, int col );


};

namespace prop
{
/** Property types supported */
enum class ePropertyType
{
    Str,
    Int,
    Dbl,
    Bool,
    Enm,
    Cat,
};

/** Property base class

*/

class property_base
{
public:
    std::string myName;
    std::string myLabel;

    /** CTOR
        @param[in] name must be unique
        @param[in] label to display, need not be unique
        @param[in] type of property
    */
    property_base(
        const std::string name,
        const std::string label,
        ePropertyType type )
        : myName( name )
        , myLabel( label )
        , myType( type )
        , myCatIndex( 0 )
    {

    }

    virtual std::string ValueAsString() const = 0;

    virtual void SetValue( const std::string& sv ) = 0;

    /** Edit option value
        @return new value as string

        This is a pure virtual function
        that must be reimplemented for each specialized property
        to pop up a dialog prompting user for the new value.

        inputbox is very helpful for doing this
    */
    virtual std::string Edit( window wd ) = 0;

    ePropertyType Type()
    {
        return myType;
    }
    virtual std::vector< std::string > Options()
    {
        return std::vector< std::string >();
    }

    bool operator==( const std::string& name )const
    {
        return ( myName == name );
    }

    void category_index( int i )
    {
        myCatIndex = i;
    }

    int category_index()
    {
        return myCatIndex;
    }

protected:
    ePropertyType myType;
    int myCatIndex;
};

/** Class than take string values */

class text :  public property_base
{
public:
    text( const std::string& name, const std::string& sv )
        : property_base( name, name, ePropertyType::Str )
    {
        myValue = sv;
    }
    text(
        const std::string& name,
        const std::string& label,
        const std::string& sv )
        : property_base( name, label, ePropertyType::Str )
    {
        myValue = sv;
    }
    std::string ValueAsString() const
    {
        return myValue;
    }
    void SetValue( const std::string& sv )
    {
        myValue = sv;
    }

    std::string Edit( window wd );

private:
    std::string myValue;
};

/** Property that can take whole number values */

class integer :  public property_base
{
public:
    integer( const std::string& name, int v )
        : property_base( name, name, ePropertyType::Str )
    {
        myValue = v;
    }
    integer(
        const std::string& name,
        const std::string& label,
        int v )
        : property_base( name, label, ePropertyType::Str )
    {
        myValue = v;
    }
    std::string ValueAsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = atoi(sv.c_str() );
    }
    void SetValue( int v)
    {
        myValue = v;
    }
    std::string Edit( window wd );

private:
    int myValue;
};

/** Property that takes double floating point values */

class real :  public property_base
{
public:
    real( const std::string& name, double v )
        : property_base( name, name, ePropertyType::Str )
    {
        myValue = v;
    }
    real(
        const std::string& name,
        const std::string& label,
        double v )
        : property_base( name, label, ePropertyType::Str )
    {
        myValue = v;
    }
    std::string ValueAsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = atof(sv.c_str() );
    }
    void SetValue( int v)
    {
        myValue = v;
    }
    std::string Edit( window wd );

private:
    double myValue;
};

/** Separator marking start of new category or group which can be collapsed.

Unfortunately categories can only be collapsed by the user
http://nanapro.org/en-us/forum/index.php?u=/topic/184/gglistbox-category-feature-requests

*/
class category : public property_base
{
public:
    /** CTOR
    @param[in] name of category
    */
    category( const std::string& name )
        : property_base( name, name, ePropertyType::Cat )
    {
    }
    std::string ValueAsString() const
    {
        return myName;
    }

    /** Categories do not have values, NOP function to satisfy compiler */
    void SetValue( const std::string& sv )
    {}

    /** Categories cannot be edited, NOP function to satisfy compiler */
    std::string Edit( window wd )
    {
        return "";
    }

};

/** Property that can be true or false */
class truefalse : public property_base
{
public:

    truefalse( const std::string& name, bool f )
        : property_base( name, name, ePropertyType::Bool )
    {
        myValue = f;
    }
    truefalse(
        const std::string& name,
        const std::string& label,
        bool f )
        : property_base( name, label, ePropertyType::Bool )
    {
        myValue = f;
    }
    std::string ValueAsString() const
    {
        if( myValue )
            return "true";
        return "false";
    }
    void SetValue( const std::string& sv )
    {
        if( sv == "true" )
            myValue = true;
        else
            myValue = false;
    }

    std::string Edit( window wd );

private:
    bool myValue;
};

/** Property that can take on one of a defined set of string values */

class options : public property_base
{
public:
    options( const std::string& name,
             const std::vector< std::string >& vopts )
        : property_base( name, name, ePropertyType::Enm )
        , myValue( vopts )
        , mySelection( 0 )
    {

    }
    std::string ValueAsString() const
    {
        if( 0 > mySelection || mySelection >= (int)myValue.size() )
            return "";
        return myValue[ mySelection ];
    }

    std::vector< std::string > Options()
    {
        return myValue;
    }

    /** Set value to one of the options
        @param[in] sv option string to select

        If sv is not equal to any of the options
        then the first option is selected
    */
    void SetValue( const std::string& sv )
    {
        auto it = std::find(
                      myValue.begin(),
                      myValue.end(),
                      sv );
        if( it == myValue.end() )
            mySelection = 0;
        else
            mySelection = it - myValue.begin();
    }

    std::string Edit( window wd );

private:
    std::vector< std::string > myValue;
    int mySelection;
};




/** vector of pointers to properties */
typedef std::shared_ptr< property_base > prop_t;
typedef std::vector< prop_t > vector_t;

/** Property grid for handling name/value pairs */

class grid : public nana::grid
{
public:
    grid( window wd, const rectangle& r = rectangle());

    /** Add properties vector vector
        @param[in] v vector of properties

        The properties in this vector will be used to initialize the grid.
        When user edits property values in the grid
        the property values in this vector will be updated.

        A pointer to the vector is stored, so the calling code
        must ensure that the vector does not go out of scope
        before the grid does
     */
    void Set( vector_t& v );

    /** Collapse or expand a category

    @param[in] category_name
    @param[in] fCollapse true for collapse, false for expand, default is true
    */
    void Collapse(
        const std::string& category_name,
        bool fCollapse = true );

    /** Change value of existing property */
//    void Set(
//        const std::string& name,
//        const std::string& value );

//    template <class T>
//    void Set( const property<T>& prop )
//    {
//        Set( prop.myName, prop.ValueAsString() );
//    }


private:

    /// pointer to external property vector
    vector_t * myVP;

};
}
}
