open Fest;

let burger: Item.Burger.t = {
  lettuce: false,
  onions: 0,
  cheese: 0,
  tomatoes: false,
  bacon: 0,
};

test("0 burgers, no discount", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurgers([
         Hotdog,
         Sandwich(Ham),
         Sandwich(Turducken),
       ]),
       Error(`NeedTwoBurgers),
     )
);

test("1 burger, no discount", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurger([Hotdog, Sandwich(Ham), Burger(burger)]),
       Error(`NeedOneBurger),
     )
);

test("2 burgers of same price, discount", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurger([
         Hotdog,
         Burger(burger),
         Sandwich(Ham),
         Burger(burger),
       ]),
       Ok(15.),
     )
);

test("2 burgers of different price, discount of cheaper one", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurger([
         Hotdog,
         Burger({...burger, tomatoes: true}), // 15.05
         Sandwich(Ham),
         Burger({...burger, bacon: 2}) // 16.00
       ]),
       Ok(15.05),
     )
);

test("3 burgers of different price, return Some(15.15)", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurger([
         Burger(burger), // 15
         Hotdog,
         Burger({...burger, tomatoes: true, cheese: 1}), // 15.15
         Sandwich(Ham),
         Burger({...burger, bacon: 2}) // 16.00
       ]),
       Ok(15.15),
     )
);

test("7 burgers, return Some(46.75)", () =>
  expect
  |> deepEqual(
       Discount.getFreeBurgers([
         Burger(burger), // 15
         Hotdog,
         Burger({...burger, cheese: 5}), // 15.50
         Sandwich(Unicorn),
         Burger({...burger, bacon: 4}), // 17.00
         Burger({...burger, tomatoes: true, cheese: 1}), // 15.15
         Sandwich(Ham),
         Burger({...burger, bacon: 2}), // 16.00
         Burger({...burger, onions: 6}), // 16.20
         Sandwich(Portabello),
         Burger({...burger, tomatoes: true}) // 15.05
       ]),
       Ok(46.75),
     )
);

module HalfOff = {
  test("No burger has 1+ of every topping, return None", () =>
    expect
    |> deepEqual(
         Discount.getHalfOff([
           Hotdog,
           Sandwich(Portabello),
           Burger({
             lettuce: true,
             tomatoes: true,
             cheese: 1,
             onions: 1,
             bacon: 0,
           }),
         ]),
         Error(`NeedMegaBurger),
       )
  );

  test("One burger has 1+ of every topping, return Some", () => {
    let items = [
      Item.Hotdog,
      Sandwich(Portabello),
      Burger({lettuce: true, tomatoes: true, cheese: 1, onions: 1, bacon: 2}),
    ];
    expect
    |> deepEqual(
         Discount.getHalfOff(items),
         {
           let sum =
             items
             |> ListLabels.fold_left(~init=0.0, ~f=(total, item) =>
                  total +. Item.toPrice(item)
                );
           Ok(sum /. 2.0);
         },
       );
  });

  test(
    "HALF promo code returns getHalfOff on May 28 but not other days of May",
    () => {
    for (dayOfMonth in 1 to 31) {
      let date =
        Js.Date.makeWithYMD(
          ~year=2024.,
          ~month=4.0,
          ~date=float_of_int(dayOfMonth),
        );

      expect
      |> deepEqual(
           Discount.getDiscountFunction("HALF", date),
           dayOfMonth == 28 ? Ok(Discount.getHalfOff) : Error(ExpiredCode),
         );
    }
  });
};

module GetDiscount = {
  test("Invalid promo code return Error", () => {
    let date = Js.Date.make();
    ["", "FREEDOM", "UNICORN", "POO"]
    |> List.iter(code =>
         expect
         |> deepEqual(
              Discount.getDiscountFunction(code, date),
              Error(InvalidCode),
            )
       );
  });

  test("FREE promo code works in May but not other months", () => {
    List.init(12, i => i)
    |> List.iter(month => {
         let date =
           Js.Date.makeWithYMD(
             ~year=2024.,
             ~month=float_of_int(month),
             ~date=10.,
           );

         expect
         |> deepEqual(
              Discount.getDiscountFunction("FREE", date),
              month == 4 ? Ok(Discount.getFreeBurgers) : Error(ExpiredCode),
            );
       })
  });
};

module SandwichHalfOff = {
  test("Not all sandwiches, return Error", () =>
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff([
           Sandwich(Unicorn),
           Hotdog,
           Sandwich(Portabello),
           Sandwich(Ham),
         ]),
         Error(`MissingSandwichTypes),
       )
  );

  test("All sandwiches, return Ok", () => {
    let items = [
      Item.Sandwich(Turducken),
      Hotdog,
      Sandwich(Portabello),
      Burger({lettuce: true, tomatoes: true, cheese: 1, onions: 1, bacon: 2}),
      Sandwich(Unicorn),
      Sandwich(Ham),
    ];
    expect
    |> deepEqual(
         Discount.getSandwichHalfOff(items),
         {
           // Don't use hardcoded value since Item.toPrice is non-deterministic
           let sum =
             items |> List.map(Item.toPrice) |> List.fold_left((+.), 0.0);
           Ok(sum /. 2.0);
         },
       );
  });
};

test(
  "HALF promo code returns getSandwichHalfOff on Nov 3 but not other days of Nov",
  () => {
  for (dayOfMonth in 1 to 30) {
    let date =
      Js.Date.makeWithYMD(
        ~year=2024.,
        ~month=10.0,
        ~date=float_of_int(dayOfMonth),
      );

    expect
    |> deepEqual(
         Discount.getDiscountFunction("HALF", date),
         dayOfMonth == 3
           ? Ok(Discount.getSandwichHalfOff) : Error(ExpiredCode),
       );
  }
});
