#include <nana/gui.hpp>
#include <nana/gui/widgets/button.hpp>
#include <grid.hpp>

using namespace nana;

/** read properties
    @param[out] properties A vector of properties to be initialized

    In production code, this would read the properties from a file or database
    For testing we will simply create some arbitrary properties
    */

void Read( prop::vector_t& vp)
{
    using namespace prop;
    vp.emplace_back(new property<std::string>( "A", "10" ));
    vp.emplace_back(new property<int>( "B", 99 ));
    vp.emplace_back(new property<double>( "C", 0.42 ));
    vp.emplace_back(new property<bool>( "Enable Feature", false ));
}

/** Save properties
    @param[in] properties A vector of properties

    In productoion code, this would write the properties out to a file or database.
    For testing we will simply dispay them in a message box
*/
void Save( const prop::vector_t& vp)
{
    msgbox mb;
    for( auto prop : vp )
    {
        mb << prop->myName << " "
           << prop->ValueAsString() << " | ";
    }
    mb();
}
int main()
{

    try
    {
        form fm;

        // construct property grid
        prop::grid pg( fm, nana::rectangle(10, 25, 280, 300 ));

        // construct vector to store properties
        // as we work with them
        prop::vector_t vp;

        //read initial properties
        Read( vp );

        // Place properties into grid
        pg.Add( vp );

        // Button to save the edited properties
        button save( fm,  nana::rectangle(60, 5, 50, 20 ));
        save.caption("SAVE");
        save.events().click([vp]
        {
            // user has clicked save button
            // save the properties with their edited values
            Save( vp );
        });

        // show the user what we have
        fm.show();

        // let user edit and save properties
        exec();
    }

    catch( std::runtime_error& e )
    {
        // horrible error somewhere
        msgbox mb( e.what() );
        mb();
    }
}

