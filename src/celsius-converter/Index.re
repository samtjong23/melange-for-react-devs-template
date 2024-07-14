module App = {
  [@react.component]
  let make = () =>
    <div
      className="min-h-screen flex flex-col items-center justify-center bg-gray-200 p-4">
      <h1 className="text-3xl font-bold mb-6 text-center text-blue-600">
        {React.string("Celsius Converter")}
      </h1>
      <CelsiusConverter />
    </div>;
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <App />);
};
