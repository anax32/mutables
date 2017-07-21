#include "patterns/touch.h"
#include "structures/parameterset.h"
#include "structures/callback.h"
#include "structures/callbackset.h"
#include "structures/bounds.h"

#include "mutables/types.h"
#include "mutables/tokens.h"
//#include "mutables/propertyset.h"
#include "mutables/description.h"
#include "mutables/property.h"

#include "mutables/validators.h"
#include "mutables/descriptionset.h"

#include "../../test/tests.h"

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
