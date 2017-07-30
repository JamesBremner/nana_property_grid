namespace nana {
namespace prop
{
/** Property types supported */
enum class eType
{
    Str,        // string
    Int,        // integer
    Dbl,        // double float
    Bool,       // boolean
    Enm,        // choice of strings
    Cat,        // category divider
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
        eType type )
        : myName( name )
        , myLabel( label )
        , myType( type )
        , myCatIndex( 0 )
    {

    }

    /** Get option value as a string
        @return value as string
        This is a pure virtual function
        that must be reimplemented for each specialized property.
    */
    virtual std::string ValueAsString() const = 0;

    /** Set option value from a string
        @param[in] sv value as string
        This is a pure virtual function
        that must be reimplemented for each specialized property.
    */
    virtual void SetValue( const std::string& sv ) = 0;

    /** Edit option value
        @return new value as string

        This is a pure virtual function
        that must be reimplemented for each specialized property
        to pop up a dialog prompting user for the new value.

        inputbox is very helpful for doing this
    */
    virtual std::string Edit( nana::window wd ) = 0;

    /** Get property type
        @return property type as one of the enumerated types
    */
    eType Type()
    {
        return myType;
    }

    /** Get the options
        @return vector of option strings

        By default this returns an empty string
        The options property type overwites this to return the actual sptions
    */
    virtual std::vector< std::string > Options()
    {
        return std::vector< std::string >();
    }

    /** Property is equal to string if string is the unique name of the property
    */
    bool operator==( const std::string& name )const
    {
        return ( myName == name );
    }

    /** Set index of category holding this property */
    void category_index( int i )
    {
        myCatIndex = i;
    }

    /** Get index of category holding this property */
    int category_index()
    {
        return myCatIndex;
    }

protected:
    eType myType;
    int myCatIndex;
};

/** Property that takes a string values */

class text :  public property_base
{
public:
    text( const std::string& name, const std::string& sv )
        : property_base( name, name, eType::Str )
    {
        myValue = sv;
    }
    text(
        const std::string& name,
        const std::string& label,
        const std::string& sv )
        : property_base( name, label, eType::Str )
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

    std::string Edit( nana::window wd );

private:
    std::string myValue;
};

/** Property that can take whole number values */

class integer :  public property_base
{
public:
    integer( const std::string& name, int v )
        : property_base( name, name, eType::Str )
    {
        myValue = v;
    }
    integer(
        const std::string& name,
        const std::string& label,
        int v )
        : property_base( name, label, eType::Str )
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
    std::string Edit( nana::window wd );

private:
    int myValue;
};

/** Property that takes double floating point values */

class real :  public property_base
{
public:
    real( const std::string& name, double v )
        : property_base( name, name, eType::Str )
    {
        myValue = v;
    }
    real(
        const std::string& name,
        const std::string& label,
        double v )
        : property_base( name, label, eType::Str )
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
    std::string Edit( nana::window wd );

private:
    double myValue;
};

/** Separator marking start of new category or group which can be collapsed.
*/
class category : public property_base
{
public:
    /** CTOR
    @param[in] name of category
    */
    category( const std::string& name )
        : property_base( name, name, eType::Cat )
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
    std::string Edit( nana::window wd )
    {
        return "";
    }

};

/** Property that can be true or false */
class truefalse : public property_base
{
public:

    truefalse( const std::string& name, bool f )
        : property_base( name, name, eType::Bool )
    {
        myValue = f;
    }
    truefalse(
        const std::string& name,
        const std::string& label,
        bool f )
        : property_base( name, label, eType::Bool )
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

    std::string Edit( nana::window wd );

private:
    bool myValue;
};

/** Property that can take on one of a defined set of string values */

class options : public property_base
{
public:
    options( const std::string& name,
             const std::vector< std::string >& vopts )
        : property_base( name, name, eType::Enm )
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

    std::string Edit( nana::window wd );

private:
    std::vector< std::string > myValue;
    int mySelection;
};

}
}

