#include <nana/gui.hpp>
#include <nana/gui/widgets/checkbox.hpp>
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
        auto sp = selected();
        if( sp.size() != 1 )
            return;
        std::string propName = at(sp[0]).text(0);
        auto it = myMap.find( propName );
        if( it == myMap.end() )
            return;
        int propIndex = it->second;
        switch( myVP->at( propIndex )->Type() )
        {

        default:
        {
            inputbox::text value(
                propName,
                at(sp[0]).text(1) );
            inputbox inbox(wd,"Edit property value");
            if( inbox.show_modal( value ) )
            {
                // user has entered new value

                // display value
                at(sp[0]).text(1,value.value());

                // store value
                myVP->at( propIndex )->SetValue( value.value() );
            }
        }
        break;

        case ePropertyType::Bool:
        {
            std::vector< std::string> opts {"true", "false"};
            inputbox::text value(
                propName, opts);
            inputbox inbox(wd,"Edit property value");
            if( inbox.show_modal( value ) )
            {
                // display value
                at(sp[0]).text(1,value.value());

                // store value
                myVP->at( propIndex )->SetValue( value.value() );
            }
        }
        break;

        case ePropertyType::Enm:
        {
            inputbox::text value(
                propName,
                myVP->at( propIndex )->Options() );
            inputbox inbox(wd,"Edit property value");
            if( inbox.show_modal( value ) )
            {
                // display value
                at(sp[0]).text(1,value.value());

                // store value
                myVP->at( propIndex )->SetValue( value.value() );
            }
        }
        break;

        case ePropertyType::Cat:
            break;
        }
    });
}



void grid::Set( vector_t& v )
{
    myVP = &v;
    auto cat = at(0);
    for( auto& prop : *myVP )
    {

        int propCount = myMap.size();
        std::string name = prop->myName;
        if ( myMap.insert( std::make_pair( name, propCount )).second )
        {
            if( prop->Type() == ePropertyType::Cat )
            {
                // add new category
                cat = append( prop->myLabel);
            }
            else
            {
                // add new item to current category
                cat.push_back(prop->myLabel);
                cat.back().text(1,prop->ValueAsString());
            }
        }
        else
        {
            std::stringstream ss;
            ss << "property:grid.Set() Two properties have same label: ";
            ss << name;
            throw std::runtime_error( ss.str() );
        }
    }
}



void grid::Set(
    const std::string& name,
    const std::string& value )
{
    int row = myMap.find( name )->second;
    nana::grid::Set( row, 1, value );
}


}
}
