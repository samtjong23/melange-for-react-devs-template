module Sandwich = {
  type t =
    | Portabello
    | Ham
    | Unicorn
    | Turducken;

  let toPrice = (~date: Js.Date.t, t) => {
    let day = date |> Js.Date.getDay |> int_of_float;

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

  let toPrice = ({onions, cheese, tomatoes, bacon, lettuce: _}) => {
    let toppingCost = (quantity, cost) => float_of_int(quantity) *. cost;

    15.  // base cost
    +. toppingCost(onions, 0.2)
    +. toppingCost(cheese, 0.1)
    +. (tomatoes ? 0.05 : 0.0)
    +. toppingCost(bacon, 0.5);
  };

  let toEmoji = ({lettuce, tomatoes, onions, cheese, bacon}) => {
    let multiple: (string, int) => string =
      emoji =>
        fun
        | 0 => ""
        | 1 => emoji
        | count => Printf.sprintf({js|%s×%d|js}, emoji, count);

    let toppingsCount =
      (lettuce ? 1 : 0) + (tomatoes ? 1 : 0) + onions + cheese + bacon;

    toppingsCount == 0
      ? {js|🍔|js}
      : Printf.sprintf(
          {js|🍔%s{%s}|js},
          toppingsCount > 12 ? {js|🥣|js} : "",
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

let toPrice = (t, ~date: Js.Date.t) => {
  switch (t) {
  | Hotdog => 5.
  | Sandwich(sandwich) => Sandwich.toPrice(sandwich, ~date)
  | Burger(burger) => Burger.toPrice(burger)
  };
};

let toEmoji =
  fun
  | Hotdog => {js|🌭|js}
  | Burger(burger) => Burger.toEmoji(burger)
  | Sandwich(sandwich) => Sandwich.toEmoji(sandwich);
