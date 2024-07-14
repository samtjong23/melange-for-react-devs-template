let getValueFromEvent = (evt): string => React.Event.Form.target(evt)##value;

let convert = celsius => 9.0 /. 5.0 *. celsius +. 32.0;

[@react.component]
let make = () => {
  let (celsius, setCelsius) = React.useState(() => "");

  <div
    className="p-4 flex items-center justify-center bg-gray-100 rounded-md shadow-md w-128">
    <div className="relative flex items-center">
      <input
        className="p-2 pr-12 border border-gray-300 rounded-md text-center"
        value=celsius
        onChange={evt => {
          let newCelsius = getValueFromEvent(evt);
          setCelsius(_ => newCelsius);
        }}
      />
      <span className="absolute right-2 text-gray-500">
        {React.string({js|°C|js})}
      </span>
    </div>
    <span className="mx-4 text-lg font-medium">
      {React.string({js|=|js})}
    </span>
    <div className="text-lg font-medium">
      {(
         String.trim(celsius) == ""
           ? {js|?°F|js}
           : (
             switch (celsius |> float_of_string_opt |> Option.map(convert)) {
             | None => "Error"
             | Some(fahrenheit) when fahrenheit < (-128.6) => {js|Unreasonably cold 🥶|js}
             | Some(fahrenheit) when fahrenheit > 212.0 => {js|Unreasonably hot 🥵|js}
             | Some(fahrenheit) =>
               Js.Float.toFixed(fahrenheit, ~digits=2) ++ {js| °F|js}
             }
           )
       )
       |> React.string}
    </div>
  </div>;
};
