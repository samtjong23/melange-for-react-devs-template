/** Get string value from the given event's target */
let getValueFromEvent = (evt): string => React.Event.Form.target(evt)##value;

/** Alias for [React.string] */
let s = React.string;

/** Render a list of [React.element]s */
let list = list => list |> Stdlib.Array.of_list |> React.array;

/** Render a float as currency */
let currency = value => value |> Js.Float.toFixed(~digits=2) |> React.string;

/** Like [React.useState] but doesn't use callback functions */
let useStateValue = initial =>
  React.useReducer((_state, newState) => newState, initial);

/** For converting option(int) to string */
let optionIntToString = (optInt: option(int)): string =>
  switch (optInt) {
  | Some(value) => Int.to_string(value)
  | None => ""
  };

/** For getting value from an option, or returning a default value if None */
let getWithDefault = (option, default) => {
  switch (option) {
  | Some(value) => value
  | None => default
  };
};
