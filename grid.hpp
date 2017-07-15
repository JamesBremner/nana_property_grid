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

    void ColTitle( int col, const std::string& value );
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

template <class T>
class value
{
public:

    T myValue;
    int mySelected;

    value()
        : mySelected( 0 )
    {

    }

    std::string AsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = sv;
    }
    ePropertyType Type()
    {
        return ePropertyType::Str;
    }

    std::vector< std::string > Options()
    {
        return std::vector< std::string >();
    }
};

template <>
class value<int>
{
public:

    int myValue;
    int mySelected;

    std::string AsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = atoi( sv.c_str() );
    }
    ePropertyType Type()
    {
        return ePropertyType::Int;
    }
    std::vector< std::string > Options()
    {
        return std::vector< std::string >();
    }

};

template <>
class value<double>
{
public:

    double myValue;
    int mySelected;

    std::string AsString() const
    {
        std::stringstream ss;
        ss << myValue;
        return ss.str();
    }
    void SetValue( const std::string& sv )
    {
        myValue = atof( sv.c_str() );
    }
    ePropertyType Type()
    {
        return ePropertyType::Dbl;
    }
    std::vector< std::string > Options()
    {
        return std::vector< std::string >();
    }
};

template <>
class value<bool>
{
public:

    bool myValue;
    int mySelected;

    std::string AsString() const
    {
        if( myValue )
            return "true";
        return "false";
    }
    void SetValue( const std::string& sv )
    {
        myValue = ( sv == "true" );
    }
    ePropertyType Type()
    {
        return ePropertyType::Bool;
    }
    std::vector< std::string > Options()
    {
        return std::vector< std::string >();
    }
};

/// Specialisation for enumerated string value

template <>
class value<std::vector<std::string>>
{
public:

    std::vector<std::string> myValue;
    int mySelected;

    value()
        : mySelected( 0 )
    {
    }

    std::string AsString() const
    {
        if( ( ! myValue.size() ) ||
                0 > mySelected  ||
                mySelected >=  myValue.size() )
            return "";
        return myValue[ mySelected];
    }
    void SetValue( const std::string& sv )
    {
        auto it = std::find(
                      myValue.begin(),
                      myValue.end(),
                      sv );
        if( it == myValue.end() )
            mySelected = 0;
        else
            mySelected = it - myValue.begin();
    }

    ePropertyType Type()
    {
        return ePropertyType::Enm;
    }
    std::vector< std::string > Options()
    {
        return myValue;
    }
};

/** Property base class

This non-templated base class allows pointers to properties of any type
to be stored in a vector, avoiding the problem of 'type slicing'
cf https://stackoverflow.com/a/16527721/16582
*/

class property_base
{
public:
    std::string myName;
    std::string myLabel;

    property_base(
        const std::string name,
        const std::string label )
        : myName( name )
        , myLabel( label )
    {

    }

    virtual std::string ValueAsString() const = 0;
    virtual void SetValue( const std::string& sv ) = 0;
    virtual ePropertyType Type() = 0;
    virtual std::vector< std::string > Options() = 0;
};

/** Property -  a name value pair with templated value type */

template <class T>
class property : public property_base
{
public:

    value<T> myValue;

    /** CTOR
        @param[in] name of property, must be unique
        @param[in] label for property, can be duplicated
        @param[in] value
    */
    property(
        const std::string& name,
        const std::string& label,
        const T& value )
        : property_base( name, label )
    {
        myValue.myValue = value;
    }

    /** CTOR
        @param[in] name and label of property, must be unique
        @param[in] value
    */
    property(
        const std::string& name,
        const T& value )
        : property_base( name, name )
    {
        myValue.myValue = value;
    }
    std::string ValueAsString() const
    {
        return myValue.AsString();
    }

    void SetValue( const std::string& sv )
    {
        myValue.SetValue( sv );
    }

    ePropertyType Type()
    {
        return myValue.Type();
    }

    std::vector< std::string > Options()
    {
        return myValue.Options();
    }

};

class category : public property_base
{
public:
        /** CTOR
        @param[in] name and label of property, must be unique
    */
    category( const std::string& name )
    : property_base(name,name)
    {
    }
    std::string ValueAsString() const
    {
        return myName;
    }

    void SetValue( const std::string& sv )
    {}

    virtual ePropertyType Type() { return ePropertyType::Cat; }
    virtual std::vector< std::string > Options() {  return std::vector< std::string >(); }
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

    /** Change value of existing property */
//    void Set(
//        const std::string& name,
//        const std::string& value );

    template <class T>
    void Set( const property<T>& prop )
    {
        Set( prop.myName, prop.ValueAsString() );
    }


private:

    /// pointer to external property vector
    vector_t * myVP;

};
}
}
