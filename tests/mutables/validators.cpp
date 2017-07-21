#include "patterns/touch.h"
#include "structures/parameterset.h"
#include "structures/callback.h"
#include "structures/callbackset.h"
#include "structures/bounds.h"

#include "mutables/types.h"
#include "mutables/tokens.h"
#include "mutables/description.h"
#include "mutables/property.h"

#include "mutables/validators.h"

#include "../../test/tests.h"

void validators_create_test ()
{
  auto p = Mutables::Property ();
}

void mutables_validators_tests ()
{
  TEST(validators_create_test);
}

int main (int argc, char** argv)
{
  TEST_GROUP(mutables_validators_tests);
}
