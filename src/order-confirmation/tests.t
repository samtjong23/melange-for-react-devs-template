Sandwich tests
  $ node ./output/src/order-confirmation/SandwichTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: Item.Sandwich.toEmoji
  ok 1 - Item.Sandwich.toEmoji
    ---
    ...
  # Subtest: Item.Sandwich.toPrice
  ok 2 - Item.Sandwich.toPrice
    ---
    ...
  # Subtest: Item.Sandwich.toPrice returns lower price for Turducken on Tuesdays
  ok 3 - Item.Sandwich.toPrice returns lower price for Turducken on Tuesdays
    ---
    ...
  1..3
  # tests 3
  # suites 0
  # pass 3
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0

Burger tests
  $ node ./output/src/order-confirmation/BurgerTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: A fully-loaded burger
  ok 1 - A fully-loaded burger
    ---
    ...
  # Subtest: Burger with 0 of onions, cheese, or bacon doesn't show those emoji
  ok 2 - Burger with 0 of onions, cheese, or bacon doesn't show those emoji
    ---
    ...
  # Subtest: Burger with 1 of onions, cheese, or bacon should show just the emoji without Ã
  ok 3 - Burger with 1 of onions, cheese, or bacon should show just the emoji without Ã
    ---
    ...
  # Subtest: Burger with 2 or more of onions, cheese, or bacon should show Ã
  ok 4 - Burger with 2 or more of onions, cheese, or bacon should show Ã
    ---
    ...
  # Subtest: Burger with more than 12 toppings should also show bowl emoji
  ok 5 - Burger with more than 12 toppings should also show bowl emoji
    ---
    ...
  1..5
  # tests 5
  # suites 0
  # pass 5
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0

Discount tests
  $ node ./output/src/order-confirmation/DiscountTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: 0 burgers, no discount
  ok 1 - 0 burgers, no discount
    ---
    ...
  # Subtest: 1 burger, no discount
  ok 2 - 1 burger, no discount
    ---
    ...
  # Subtest: 2 burgers of same price, discount
  ok 3 - 2 burgers of same price, discount
    ---
    ...
  # Subtest: 2 burgers of different price, discount of cheaper one
  ok 4 - 2 burgers of different price, discount of cheaper one
    ---
    ...
  # Subtest: 3 burgers of different price, return Ok(15.15)
  ok 5 - 3 burgers of different price, return Ok(15.15)
    ---
    ...
  # Subtest: 7 burgers, return Ok(46.75)
  ok 6 - 7 burgers, return Ok(46.75)
    ---
    ...
  # Subtest: No burger has 1+ of every topping, return Error(`NeedMegaBurger)
  ok 7 - No burger has 1+ of every topping, return Error(`NeedMegaBurger)
    ---
    ...
  # Subtest: One burger has 1+ of every topping, return Ok(15.675)
  ok 8 - One burger has 1+ of every topping, return Ok(15.675)
    ---
    ...
  # Subtest: Not all sandwiches, return Error
  ok 9 - Not all sandwiches, return Error
    ---
    ...
  # Subtest: All sandwiches, return Ok
  ok 10 - All sandwiches, return Ok
    ---
    ...
  # Subtest: Invalid promo code return Error
  ok 11 - Invalid promo code return Error
    ---
    ...
  # Subtest: FREE promo code works in May but not other months
  ok 12 - FREE promo code works in May but not other months
    ---
    ...
  # Subtest: HALF promo code returns getHalfOff on May 28 but not other days of May
  ok 13 - HALF promo code returns getHalfOff on May 28 but not other days of May
    ---
    ...
  # Subtest: HALF promo code returns getSandwichHalfOff on Nov 3 but not other days of Nov
  ok 14 - HALF promo code returns getSandwichHalfOff on Nov 3 but not other days of Nov
    ---
    ...
  1..14
  # tests 14
  # suites 0
  # pass 14
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0

Create tests
  $ node ./output/src/order-confirmation/CreateTests.mjs | sed '/duration_ms/d'
  TAP version 13
  # Subtest: AddError actually adds a new error to error list
  ok 1 - AddError actually adds a new error to error list
    ---
    ...
  # Subtest: RemoveError actually removes error from error list
  ok 2 - RemoveError actually removes error from error list
    ---
    ...
  # Subtest: RemoveError removes the right error when there are multiple error in error list
  ok 3 - RemoveError removes the right error when there are multiple error in error list
    ---
    ...
  # Subtest: ClearErrors removes all errors from error list
  ok 4 - ClearErrors removes all errors from error list
    ---
    ...
  # Subtest: NotEmpty rule with none bool value should return error message
  ok 5 - NotEmpty rule with none bool value should return error message
    ---
    ...
  # Subtest: NotEmpty rule with non none bool value should not return error message
  ok 6 - NotEmpty rule with non none bool value should not return error message
    ---
    ...
  # Subtest: NotEmpty rule with none int value should return error message
  ok 7 - NotEmpty rule with none int value should return error message
    ---
    ...
  # Subtest: NotEmpty rule with non none int value should not return error message
  ok 8 - NotEmpty rule with non none int value should not return error message
    ---
    ...
  # Subtest: NotNegative rule with none int value should return NotEmpty error message
  ok 9 - NotNegative rule with none int value should return NotEmpty error message
    ---
    ...
  # Subtest: NotNegative rule with negative int value should return error message
  ok 10 - NotNegative rule with negative int value should return error message
    ---
    ...
  # Subtest: NotNegative rule with non negative int value should not return error message
  ok 11 - NotNegative rule with non negative int value should not return error message
    ---
    ...
  # Subtest: Single validation passes
  ok 12 - Single validation passes
    ---
    ...
  # Subtest: Single validation fails
  ok 13 - Single validation fails
    ---
    ...
  # Subtest: Multiple validations all pass
  ok 14 - Multiple validations all pass
    ---
    ...
  # Subtest: Multiple validations all fail
  ok 15 - Multiple validations all fail
    ---
    ...
  # Subtest: Multiple validations some pass some fail
  ok 16 - Multiple validations some pass some fail
    ---
    ...
  # Subtest: Empty list/no validations
  ok 17 - Empty list/no validations
    ---
    ...
  # Subtest: Field does not exist in an empty error list
  ok 18 - Field does not exist in an empty error list
    ---
    ...
  # Subtest: Field exists in a single element error list
  ok 19 - Field exists in a single element error list
    ---
    ...
  # Subtest: Field does not exist in a single element error list
  ok 20 - Field does not exist in a single element error list
    ---
    ...
  # Subtest: Field exists in a multiple elements error list
  ok 21 - Field exists in a multiple elements error list
    ---
    ...
  # Subtest: Field does not exist in a multiple elements error list
  ok 22 - Field does not exist in a multiple elements error list
    ---
    ...
  1..22
  # tests 22
  # suites 0
  # pass 22
  # fail 0
  # cancelled 0
  # skipped 0
  # todo 0
