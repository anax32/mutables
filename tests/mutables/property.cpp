#include "patterns/touch.h"
#include "structures/parameterset.h"
#include "structures/callback.h"
#include "structures/bounds.h"
#include "mutables/property.h"

void property_create_test ()
{
	auto p = Mutables::Property ();
}

void mutables_property_tests ()
{
	TEST(property_create_test);
}

int main (int argc, char** argv)
{
	TEST_GROUP(mutables_property_tests);
}