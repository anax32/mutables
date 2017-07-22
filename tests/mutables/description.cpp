#include "patterns/touch.h"
#include "structures/parameterset.h"
#include "structures/callback.h"
#include "structures/callbackset.h"
#include "structures/bounds.h"

#include "mutables/types.h"
#include "mutables/description.h"

#include "mutables/tokens.h"
#include "mutables/property.h"
#include "mutables/validators.h"
#include "mutables/drawers.h"
#include "mutables/mutators.h"
#include "mutables/descriptionset.h"

#include "maketest.h"

void description_create_test ()
{
  auto d = Mutables::Description ();

  assert_are_equal (d.displayName (), Mutables::UNSET);
  assert_are_equal (d.displayName (), Mutables::UNSET);
  assert_are_equal (d.displayName (), Mutables::UNSET);
  assert_are_equal (d.displayName (), Mutables::UNSET);
}
void description_create_details_test ()
{
  auto d = Mutables::Description ("display", "description", "value", "access");

  assert_are_equal (d.displayName (), "display");
  assert_are_equal (d.description (), "description");
  assert_are_equal (d.defaultValue (), "value");
  assert_are_equal (d.defaultAccess (), "access");
}
void description_set_details_test ()
{
  auto d = Mutables::Description ();

  assert_are_equal (d.displayName (), Mutables::UNSET);
  assert_are_equal (d.displayName (), Mutables::UNSET);
  assert_are_equal (d.displayName (), Mutables::UNSET);
  assert_are_equal (d.displayName (), Mutables::UNSET);

  d.set ("display", "description", "value", "access");

  assert_are_equal (d.displayName (), "display");
  assert_are_equal (d.description (), "description");
  assert_are_equal (d.defaultValue (), "value");
  assert_are_equal (d.defaultAccess (), "access");
}
void description_set_values_test ()
{
  auto d = Mutables::Description ("d", "desc", "val", "access");
  auto v = IntegerSet
  {
    {"test1", 1},
    {"test2", 2},
    {"test3", 3}
  };

  assert_are_equal (0, d.values().parameterCount ());

  d.setValues<IntegerSet> (v.begin (), v.end ());;

  assert_are_equal (3, d.values().parameterCount ());
  assert_are_equal (1, d.values ()["test1"]);
  assert_are_equal (2, d.values ()["test2"]);
  assert_are_equal (3, d.values ()["test3"]);
}
void description_set_values_inline_test ()
{
  auto d = Mutables::Description ("d", "desc", "val", "access");

  assert_are_equal (0, d.values().parameterCount ());

  d.setValues<IntegerSet> (IntegerSet {{"test1", 1}, {"test2", 2}, {"test3", 3}});

  assert_are_equal (3, d.values().parameterCount ());
  assert_are_equal (1, d.values ()["test1"]);
  assert_are_equal (2, d.values ()["test2"]);
  assert_are_equal (3, d.values ()["test3"]);
}
void description_append_values_test ()
{
  auto d = Mutables::Description ("d", "desc", "val", "access");

  assert_are_equal (0, d.values().parameterCount ());

  d.setValues<IntegerSet> (IntegerSet {{"test1", 1}, {"test2", 2}, {"test3", 3}});

  assert_are_equal (3, d.values().parameterCount ());
  assert_are_equal (1, d.values ()["test1"]);
  assert_are_equal (2, d.values ()["test2"]);
  assert_are_equal (3, d.values ()["test3"]);

  d.appendValues<IntegerSet> (IntegerSet {{"test4", 4}, {"test5", 5}, {"test6", 6}});

  assert_are_equal (6, d.values().parameterCount ());
  assert_are_equal (1, d.values ()["test1"]);
  assert_are_equal (2, d.values ()["test2"]);
  assert_are_equal (3, d.values ()["test3"]);
  assert_are_equal (4, d.values ()["test4"]);
  assert_are_equal (5, d.values ()["test5"]);
  assert_are_equal (6, d.values ()["test6"]);
}
void description_reset_values_test ()
{
  auto d = Mutables::Description ("d", "desc", "val", "access");

  assert_are_equal (0, d.values().parameterCount ());

  d.setValues<IntegerSet> (IntegerSet {{"test1", 1}, {"test2", 2}, {"test3", 3}});

  assert_are_equal (3, d.values().parameterCount ());
  assert_are_equal (1, d.values ()["test1"]);
  assert_are_equal (2, d.values ()["test2"]);
  assert_are_equal (3, d.values ()["test3"]);

  d.setValues<IntegerSet> (IntegerSet {{"test4", 4}, {"test5", 5}, {"test6", 6}});

  assert_are_equal (3, d.values().parameterCount ());
  assert_are_equal (4, d.values ()["test4"]);
  assert_are_equal (5, d.values ()["test5"]);
  assert_are_equal (6, d.values ()["test6"]);
}
void valuelist_next_value_test ()
{
  auto v = Mutables::Description::ValueList (
    {{"test1", 1}, {"test2", 2}, {"test3", 3},
     {"test4", 4}, {"test5", 5}, {"test6", 6}});

  assert_are_equal (6, v.parameterCount ());
  assert_are_equal (1, v["test1"]);
  assert_are_equal (2, v["test2"]);
  assert_are_equal (3, v["test3"]);
  assert_are_equal (4, v["test4"]);
  assert_are_equal (5, v["test5"]);
  assert_are_equal (6, v["test6"]);

  auto p = Param ("test3");

  auto q = Mutables::Description::nextValue (v, p);
  auto r = Mutables::Description::nextValue (v, q);

  assert_are_equal ("test4", q);
  assert_are_equal ("test5", r);
}
void valuelist_prev_value_test ()
{
  auto v = Mutables::Description::ValueList (
    {{"test1", 1}, {"test2", 2}, {"test3", 3},
     {"test4", 4}, {"test5", 5}, {"test6", 6}});

  assert_are_equal (6, v.parameterCount ());
  assert_are_equal (1, v["test1"]);
  assert_are_equal (2, v["test2"]);
  assert_are_equal (3, v["test3"]);
  assert_are_equal (4, v["test4"]);
  assert_are_equal (5, v["test5"]);
  assert_are_equal (6, v["test6"]);

  auto p = Param ("test3");

  auto q = Mutables::Description::previousValue (v, p);
  auto r = Mutables::Description::previousValue (v, q);

  assert_are_equal ("test2", q);
  assert_are_equal ("test1", r);
}
void valuelist_next_value_wrap_test ()
{
  auto v = Mutables::Description::ValueList (
    {{"test1", 1}, {"test2", 2}, {"test3", 3},
     {"test4", 4}, {"test5", 5}, {"test6", 6}});

  assert_are_equal (6, v.parameterCount ());
  assert_are_equal (1, v["test1"]);
  assert_are_equal (2, v["test2"]);
  assert_are_equal (3, v["test3"]);
  assert_are_equal (4, v["test4"]);
  assert_are_equal (5, v["test5"]);
  assert_are_equal (6, v["test6"]);

  auto p = Param ("test5");

  auto q = Mutables::Description::nextValue (v, p);
  auto r = Mutables::Description::nextValue (v, q);

  assert_are_equal ("test6", q);
  assert_are_equal ("test1", r);
}
void valuelist_prev_value_wrap_test ()
{
  auto v = Mutables::Description::ValueList (
    {{"test1", 1}, {"test2", 2}, {"test3", 3},
     {"test4", 4}, {"test5", 5}, {"test6", 6}});

  assert_are_equal (6, v.parameterCount ());
  assert_are_equal (1, v["test1"]);
  assert_are_equal (2, v["test2"]);
  assert_are_equal (3, v["test3"]);
  assert_are_equal (4, v["test4"]);
  assert_are_equal (5, v["test5"]);
  assert_are_equal (6, v["test6"]);

  auto p = Param ("test2");

  auto q = Mutables::Description::previousValue (v, p);
  auto r = Mutables::Description::previousValue (v, q);

  assert_are_equal ("test1", q);
  assert_are_equal ("test6", r);
}

void mutables_description_tests ()
{
  TEST(description_create_test);
  TEST(description_create_details_test);
  TEST(description_set_details_test);
  TEST(description_set_values_test);
  TEST(description_set_values_inline_test);
  TEST(description_append_values_test);
  TEST(description_reset_values_test);
}

void mutables_valuelist_tests ()
{
  TEST(valuelist_next_value_test);
  TEST(valuelist_prev_value_test);
  TEST(valuelist_next_value_wrap_test);
  TEST(valuelist_prev_value_wrap_test);
}


int main (int argc, char** argv)
{
  TEST_GROUP(mutables_description_tests);
  TEST_GROUP(mutables_valuelist_tests);
}
