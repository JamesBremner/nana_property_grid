#include <nana/gui.hpp>
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
    myRowCount = rows;
    myColCount = cols;

    for( int kcol = 0; kcol < myColCount; kcol++ )
        append_header("c", 200);

    for( int krow = 0; krow < myRowCount; krow++ )
        at(0).append({"", ""});
}

void grid::ColTitle( int col, const std::string& value )
{
    column_at( col ).text( value );
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

namespace prop {

grid::grid( window wd, const rectangle& r)
    : nana::grid( wd, r )
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
            at(sp[0]).text(1) );
        inputbox inbox(wd,"Edit property value");
        if( inbox.show_modal( value ) ) {
            // user has entered new value

            // display value
            at(sp[0]).text(1,value.value());

            // store value
            myVP->at( myMap.find( at(sp[0]).text(0) )->second )->SetValue( value.value() );
        }
    });
}



void grid::Add( vector_t& v )
{
    myVP = &v;
    for( auto& prop : *myVP )
    {
        int propCount = myMap.size();
        std::string name = prop->myName;
        myMap.insert( std::make_pair( name, propCount ));
        nana::grid::Set( propCount, 0, name );
        nana::grid::Set( propCount, 1, prop->ValueAsString() );
    }
}

void grid::Set(
    const std::string& name,
    const std::string& value )
{
    int row = myMap.find( name )->second;
    nana::grid::Set( row, 1, value );
}

//std::vector< std::pair< std::string, std::string > >
//grid::AllProps()
//{
//    std::vector< std::pair< std::string, std::string > > vp;
//    for( int k = 0; k < myPropCount; k++ )
//    {
//        vp.push_back(std::make_pair(
//                         at(listbox::index_pair(0,k)).text(0),
//                         at(listbox::index_pair(0,k)).text(1) ));
//    }
//    return vp;
//}
}
}
