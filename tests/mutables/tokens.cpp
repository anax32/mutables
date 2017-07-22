#include "structures/parameterset.h"
#include "mutables/types.h"

#define TOK_FNCS
#include "mutables/tokens.h"

#include "maketest.h"

void tokens_get_single_token_test ()
{
	auto p = Param ("token");
	auto q = Param("");

	Mutables::Token::get (p, 0, q);

	assert_are_equal (q, p);
}
void tokens_get_paired_token_test ()
{
	auto p = Param ("first@second");
	auto f = Param("");
	auto s = Param("");

	Mutables::Token::get (p, 0, f);
	Mutables::Token::get (p, 1, s);

	assert_are_equal (f, "first");
	assert_are_equal (s, "second");
}
void tokens_get_integer_token_test ()
{
	auto p = Param ("1");
	int	i;

	assert_is_true (Mutables::Token::get (i, p));
	assert_are_equal (1, i);
}
void tokens_get_float_token_test ()
{
	auto p = Param ("3.14159");
	float pi;

	assert_is_true (Mutables::Token::get (pi, p));
	assert_are_equal (3.14159f, pi);
}
void tokens_get_typed_token_test ()
{
	auto p = Param ("1@3.14159");
	int	i;
	float pi;

	assert_is_true (Mutables::Token::get (i, p, 0));
	assert_is_true (Mutables::Token::get (pi, p, 1));

	assert_are_equal (1, i);
	assert_are_equal (3.14159f, pi);
}
void tokens_put_integer_token_test ()
{
	auto p = Param ("1@2@3@4");
	auto q = Param ("");
	int t[4] = {0};

	assert_is_true (Mutables::Token::put (p, 0, 10));
	assert_is_true (Mutables::Token::put (p, 1, 20));
	assert_is_true (Mutables::Token::put (p, 2, 30));
	assert_is_true (Mutables::Token::put (p, 3, 40));

	assert_is_true (Mutables::Token::get (t[0], p, 0));
	assert_is_true (Mutables::Token::get (t[1], p, 1));
	assert_is_true (Mutables::Token::get (t[2], p, 2));
	assert_is_true (Mutables::Token::get (t[3], p, 3));

	assert_are_equal (10, t[0]);
	assert_are_equal (20, t[1]);
	assert_are_equal (30, t[2]);
	assert_are_equal (40, t[3]);
}

void mutables_tokens_tests ()
{
	TEST(tokens_get_single_token_test);
	TEST(tokens_get_paired_token_test);
	TEST(tokens_get_integer_token_test);
	TEST(tokens_get_float_token_test);
	TEST(tokens_get_typed_token_test);
	TEST(tokens_put_integer_token_test);
}

int main (int argc, char** argv)
{
	TEST_GROUP(mutables_tokens_tests);
}