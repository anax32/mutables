#include "patterns/touch.h"
#include "structures/parameterset.h"
#include "structures/callback.h"
#include "structures/callbackset.h"
#include "structures/bounds.h"
#include "mutables/types.h"
#include "mutables/tokens.h"
#include "mutables/propertyset.h"
#include "mutables/property.h"

#include "../../test/test.h"

void mutables_create_test ()
{
  auto p = Mutables::PropertySet::create("test");
}

void mutables_propertyset_tests ()
{
  TEST(mutables_create_test);
}

int main (int argc, char** argv)
{
  TEST_GROUP(mutables_propertyset_tests);
}
