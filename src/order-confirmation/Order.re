type t = list(Item.t);

module OrderItem = {
  module Style = {
    let row = [%cx {|
      border-top: 1px solid lightgray;
    |}];

    let emoji = [%cx {|
      font-size: 2rem;
    |}];

    let price = [%cx
      {|
        text-align: right;
        font-size: 0.875rem;
        font-family: 'Inter', sans-serif;
      |}
    ];
  };

  [@react.component]
  let make = (~item: Item.t, ~date: Js.Date.t) =>
    <tr className=Style.row>
      <td className=Style.emoji> {item |> Item.toEmoji |> React.string} </td>
      <td className=Style.price>
        {item |> Item.toPrice(~date) |> RR.currency}
      </td>
    </tr>;
};

module Style = {
  let container = [%cx
    {|
      width: 100%;
      padding: 1rem;
      background-color: #ffffff;
      border-radius: 0.375rem;
      box-sizing: border-box;
    |}
  ];

  let table = [%cx
    {|
      width: 100%;
      border-collapse: collapse;
      margin-bottom: 1rem;
    |}
  ];

  let subtotalRow = [%cx
    {|
      display: flex;
      justify-content: space-between;
      padding: 0.5rem 0;
    |}
  ];

  let subtotalRowLeft = [%cx
    {|
      text-align: left;
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
    |}
  ];

  let subtotalRowRight = [%cx
    {|
      text-align: right;
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
    |}
  ];

  let totalRow = [%cx
    {|
      display: flex;
      justify-content: space-between;
      padding: 0.5rem 0;
    |}
  ];

  let totalRowLeft = [%cx
    {|
      text-align: left;
      font-size: 0.875rem;
      font-weight: bold;
      font-family: 'Inter', sans-serif;
    |}
  ];

  let totalRowRight = [%cx
    {|
      text-align: right;
      font-size: 0.875rem;
      font-weight: bold;
      font-family: 'Inter', sans-serif;
    |}
  ];
};

[@react.component]
let make = (~items: t, ~date: Js.Date.t) => {
  let (discountValue, setDiscountValue) = RR.useStateValue(0.);
  let total =
    items
    |> ListLabels.fold_left(~init=0., ~f=(acc, order) =>
         acc +. Item.toPrice(order, ~date)
       );

  <div className=Style.container>
    <table className=Style.table>
      <tbody>
        {items
         |> List.mapi((index, item) =>
              <OrderItem key={"item-" ++ string_of_int(index)} item date />
            )
         |> Stdlib.Array.of_list
         |> React.array}
      </tbody>
    </table>
    <div className=Style.subtotalRow>
      <div className=Style.subtotalRowLeft> {RR.s("Subtotal")} </div>
      <div className=Style.subtotalRowRight> {total |> RR.currency} </div>
    </div>
    <Promo items date setDiscountValue />
    <div className=Style.totalRow>
      <div className=Style.totalRowLeft> {RR.s("Total")} </div>
      <div className=Style.totalRowRight>
        {total -. discountValue |> RR.currency}
      </div>
    </div>
  </div>;
};
