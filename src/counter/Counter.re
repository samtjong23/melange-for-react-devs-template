[@react.component]
let make = () => {
  let (counter, setCounter) = React.useState(() => 0);

  <div
    className="p-4 flex space-x-4 items-center justify-center bg-gray-100 rounded-md shadow-md">
    <button
      className="px-4 py-2 bg-red-500 text-white font-bold rounded-md hover:bg-red-600"
      onClick={_evt => setCounter(v => v - 1)}>
      {React.string("-")}
    </button>
    <span className="text-xl font-medium">
      {counter |> Int.to_string |> React.string}
    </span>
    <button
      className="px-4 py-2 bg-green-500 text-white font-bold rounded-md hover:bg-green-600"
      onClick={_evt => setCounter(v => v + 1)}>
      {React.string("+")}
    </button>
  </div>;
};
