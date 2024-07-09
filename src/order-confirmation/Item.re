module Sandwich = {
  type t =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  let toPrice = t => {
    let day = Js.Date.make() |> Js.Date.getDay |> int_of_float;

    switch (t) {
    | Portabello
    | Ham => 10.
    | Unicorn => 80.
    | Turducken when day == 2 => 10.
    | Turducken => 20.
    };
  };

  let toEmoji = t =>
    Printf.sprintf(
      {js|🥪(%s)|js},
      switch (t) {
      | Portabello => {js|🍄|js}
      | Ham => {js|🐷|js}
      | Unicorn => {js|🦄|js}
      | Turducken => {js|🦃🦆🐓|js}
      },
    );
};

module Burger = {
  type t = {
    lettuce: bool,
    tomatoes: bool,
    onions: int,
    cheese: int,
    bacon: int,
  };

  let toPrice = ({onions, cheese, bacon, tomatoes, lettuce: _}) => {
    let toppingCost = (quantity, cost) => float_of_int(quantity) *. cost;

    15.
    +. toppingCost(onions, 0.2)
    +. toppingCost(cheese, 0.1)
    +. toppingCost(bacon, 0.5)
    +. (tomatoes ? 0.05 : 0.);
  };

  let toEmoji = ({lettuce, tomatoes, onions, cheese, bacon}) => {
    let hasToppings = () => lettuce || tomatoes || onions + cheese + bacon > 0;
    let multiple: (string, int) => string =
      emoji =>
        fun
        | 0 => ""
        | 1 => emoji
        | count => Printf.sprintf({js|%s×%d|js}, emoji, count);

    !hasToppings()
      ? {js|🍔|js}
      : Printf.sprintf(
          {js|🍔{%s}|js},
          [|
            lettuce ? {js|🥬|js} : "",
            tomatoes ? {js|🍅|js} : "",
            multiple({js|🧅|js}, onions),
            multiple({js|🧀|js}, cheese),
            multiple({js|🥓|js}, bacon),
          |]
          |> Js.Array.filter(~f=str => str != "")
          |> Js.Array.join(~sep=","),
        );
  };
};

type t =
  | Sandwich(Sandwich.t)
  | Burger(Burger.t)
  | Hotdog;

let toPrice = t => {
  switch (t) {
  | Hotdog => 5.
  | Sandwich(sandwich) => Sandwich.toPrice(sandwich)
  | Burger(burger) => Burger.toPrice(burger)
  };
};

let toEmoji =
  fun
  | Hotdog => {js|🌭|js}
  | Burger(burger) => Burger.toEmoji(burger)
  | Sandwich(sandwich) => Sandwich.toEmoji(sandwich);
