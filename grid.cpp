#include <nana/gui.hpp>
#include <grid.hpp>
namespace nana
{

gridlb::gridlb( window wd, const rectangle& r)
    : listbox( wd, r )
    , myRowCount( 0 )
    , myColCount( 0 )
{

}

void gridlb::Resize( int rows, int cols )
{
    myRowCount = rows;
    myColCount = cols;

    for( int kcol = 0; kcol < myColCount; kcol++ )
        append_header("c", 200);

    for( int krow = 0; krow < myRowCount; krow++ )
        at(0).append({"", ""});
}

void gridlb::ColTitle( int col, const std::string& value )
{
    column_at( col ).text( value );
}

void gridlb::Set( int row, int col, const std::string& value )
{
    if( ! CheckIndex( row, col ) )
        return;
    at(listbox::index_pair(0,row)).text(col,value);

}

bool gridlb::CheckIndex( int row, int col )
{
    if( 0 > row || row >= myRowCount ||
            0 > col || col >= myColCount )
        return false;
    return true;
}

property_grid::property_grid( window wd, const rectangle& r)
    : gridlb( wd, r )
    , myPropCount( 0 )
{
    Resize( 5, 2 );
    ColTitle(0,"Property");
    ColTitle(1,"Value");

    // prompt user to edit value of property clicked on
    events().click([this,wd]
    {
        auto sp = selected();
        inputbox::text value(
            at(sp[0]).text(0),
            at(sp[0]).text(1) );        inputbox inbox(wd,"Edit property value");
        if( inbox.show_modal( value ) )
            at(sp[0]).text(1,value.value());

    });
}

void property_grid::Add(
    const std::string& name,
    const std::string& value )
{
    myMap.insert( std::make_pair( name,myPropCount ));
    gridlb::Set( myPropCount, 0, name );
    gridlb::Set( myPropCount, 1, value );
    myPropCount++;
}

void property_grid::Set(
    const std::string& name,
    const std::string& value )
{
    int row = myMap.find( name )->second;
    gridlb::Set( row, 1, value );
}
}
