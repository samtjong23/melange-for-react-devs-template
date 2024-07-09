[@react.component]
let make = () => {
  let (counter, setCounter) = React.useState(() => 0);

  <div
    style={ReactDOM.Style.make(
      ~padding="1em",
      ~display="flex",
      ~gridGap="1em",
      (),
    )}>
    <button onClick={_evt => setCounter(v => v - 1)}>
      {React.string("-")}
    </button>
    {counter |> Int.to_string |> React.string}
    <button onClick={_evt => setCounter(v => v + 1)}>
      {React.string("+")}
    </button>
  </div>;
};
