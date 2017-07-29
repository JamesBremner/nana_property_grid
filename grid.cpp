#include <set>
#include <nana/gui.hpp>
#include <nana/gui/widgets/checkbox.hpp>
#include <nana/gui/widgets/group.hpp>
#include <grid.hpp>
namespace nana
{

grid::grid( window wd, const rectangle& r)
    : listbox( wd, r )
    , myRowCount( 0 )
    , myColCount( 0 )
{

}

void grid::Resize( int rows, int cols )
{
    int newRows = rows - myRowCount;
    int newCols = cols - myColCount;
    myRowCount = rows;
    myColCount = cols;

    for( int kcol = 0; kcol < newCols; kcol++ )
        append_header("c", 200);

    for( int krow = 0; krow < newRows; krow++ )
        at(0).append({"", ""});
}
void grid::ColTitle( int col, const std::string& value )
{
    column_at( col ).text( value );
}
void grid::ColWidth( int col, int width )
{
    column_at( col ).width( width );
}
void grid::Set( int row, int col, const std::string& value )
{
    if( ! CheckIndex( row, col ) )
        return;
    at(listbox::index_pair(0,row)).text(col,value);

}

bool grid::CheckIndex( int row, int col )
{
    if( 0 > row || row >= myRowCount ||
            0 > col || col >= myColCount )
        return false;
    return true;
}

namespace prop
{

std::string text::Edit( window wd )
{
    inputbox::text value(
        myName,
        myValue );
    inputbox inbox(wd,"Edit property value");
    if( inbox.show_modal( value ) )
    {
        // store value
        SetValue( value.value() );
    }
    return ValueAsString();
}

std::string integer::Edit( window wd )
{
    std::stringstream sv;
    sv << myValue;
    inputbox::text value(
        myName,
        sv.str() );
    inputbox inbox(wd,"Edit property value");
    if( inbox.show_modal( value ) )
    {
        // store value
        SetValue( value.value() );
    }
    return ValueAsString();
}

std::string real::Edit( window wd )
{
    std::stringstream sv;
    sv << myValue;
    inputbox::text value(
        myName,
        sv.str() );
    inputbox inbox(wd,"Edit property value");
    if( inbox.show_modal( value ) )
    {
        // store value
        SetValue( value.value() );
    }
    return ValueAsString();
}
std::string options::Edit( window wd )
{
    inputbox::text value(
        myName,
        Options() );
    inputbox inbox(wd,"Edit property value");
    if( inbox.show_modal( value ) )
    {
        // store value
        SetValue( value.value() );
    }
    return ValueAsString();
}

std::string truefalse::Edit( window wd )
{
    inputbox::text value(
        myName,
        std::vector< std::string > { "true", "false"} );
    inputbox inbox(wd,"Edit property value");
    if( inbox.show_modal( value ) )
    {
        // store value
        SetValue( value.value() );
    }
    return ValueAsString();
}

grid::grid( window wd, const rectangle& r)
    : nana::grid( wd, r )
{
    Resize( 0, 2 );
    ColTitle(0,"Property");
    ColTitle(1,"Value");
    ColWidth(1, 50 );

    // prompt user to edit value of property clicked on
    events().click([this,wd]
    {
        // Ensure user selected just one property
        // ( multiple property selection is ignored )
        auto sp = selected();
        if( sp.size() != 1 )
            return;

        // Edit selected property and display new value
        at(sp[0]).text(
            1,
            myVP->at( at(sp[0]).value<int>() )->Edit( wd ));

    });
}



void grid::Set( vector_t& v )
{
    myVP = &v;
    std::set< std::string > NameSet;
    int propIndex = 0;
    int catIndex = 0;
    auto cat = at(0);
    for( auto& prop : *myVP )
    {

        std::string name = prop->myName;
        if ( NameSet.insert( name ).second )
        {
            if( prop->Type() == ePropertyType::Cat )
            {
                // add new category
                cat = append( prop->myLabel);

                // suppress display of number of items in category
                cat.display_number( false );

                // store category index in property
                catIndex++;
                prop->category_index( catIndex );
            }
            else
            {
                // add new item to current category
                cat.push_back(prop->myLabel);
                cat.back().text(1,prop->ValueAsString());

                // store the index of the property in the external vector
                // as the assocaited value of the listbox item
                // so that it can be easily recovered when user clicks on property
                cat.back().value( propIndex );
            }
        }
        else
        {
            std::stringstream ss;
            ss << "property:grid.Set() Two properties have same name: ";
            ss << name;
            throw std::runtime_error( ss.str() );
        }

        propIndex++;
    }
}
void  grid::Collapse(
    const std::string& category_name,
    bool fCollapse )
{
    int row = 0;
    for( auto p : *myVP )
    {
        if( *p == category_name )
        {
            at( p->category_index() ).expanded( ! fCollapse );

            break;
        }
        row++;
    }
}


}
}
