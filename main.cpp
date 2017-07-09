#include <nana/gui.hpp>
#include <grid.hpp>

int main()
{
    using namespace nana;

    try
    {
        form fm;

        // construct property grid
        property_grid pg( fm, nana::rectangle(10, 10, 280, 300 ));

        // add some properties with initial values
        pg.Add( "A", "10");
        pg.Add( "B", "99");

        // change value of a property
        pg.Set("A","32");

        fm.show();
        exec();
    }
    catch( std::runtime_error& e )
    {
        msgbox mb( e.what() );
        mb();
    }
}

