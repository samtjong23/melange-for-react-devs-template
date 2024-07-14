module Style = {
  let container = [%cx
    {|
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      background-color: #e5e7eb;
      padding: 2rem;
    |}
  ];

  let heading = [%cx
    {|
      font-size: 2.25rem;
      font-weight: bold;
      margin-bottom: 2rem;
      text-align: center;
      color: #2563eb;
    |}
  ];

  let sectionContainer = [%cx
    {|
      width: 60%;
      background-color: #f3f4f6;
      border-radius: 0.375rem;
      box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06);
    |}
  ];

  let subheading = [%cx
    {|
      font-size: 1.875rem;
      font-weight: bold;
      padding: 0 1rem 0 1rem;
      text-align: left;
      color: #f97316;
    |}
  ];

  let content = [%cx
    {|
      width: 100%;
      padding: 0 1rem 1rem 1rem;
      box-sizing: border-box;
    |}
  ];
};

module App = {
  [@react.component]
  let make = () => {
    let date = Js.Date.fromString("2024-05-10T00:00");
    let (items: Order.t, setItems) = RR.useStateValue([]);

    <div className=Style.container>
      <h1 className=Style.heading> {React.string("Order Confirmation")} </h1>
      <div className=Style.sectionContainer>
        <h2 className=Style.subheading> {React.string("Add new items")} </h2>
        <div className=Style.content> <Create items setItems /> </div>
        <h2 className=Style.subheading> {React.string("Order List")} </h2>
        <div className=Style.content> <Order items date /> </div>
      </div>
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
