module App = {
  [@react.component]
  let make = () => {
    let date = Js.Date.fromString("2024-05-10T00:00");
    let (items: Order.t, setItems) = RR.useStateValue([]);

    <div>
      <h1> {RR.s("Add new items")} </h1>
      <Create items setItems />
      <h1> {RR.s("Promo")} </h1>
      <Promo items date={Js.Date.fromString("2024-05-10T00:00")} />
      <h1> {React.string("Order confirmation")} </h1>
      <Order items date />
    </div>;
  };
};

let node = ReactDOM.querySelector("#root");
switch (node) {
| None =>
  Js.Console.error("Failed to start React: couldn't find the #root element")
| Some(root) =>
  let root = ReactDOM.Client.createRoot(root);
  ReactDOM.Client.render(root, <App />);
};
