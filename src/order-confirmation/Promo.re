module Style = {
  let form = [%cx {|
    display: flex;
    flex-direction: column;
  |}];

  let input = [%cx
    {|
      font-family: monospace;
      text-transform: uppercase;
      text-align: center;
      border: 1px solid #d1d5db;
      border-radius: 0.375rem;
    |}
  ];

  let discountValue = [%cx
    {|
      color: red;
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
      text-align: right;
    |}
  ];

  let codeError = [%cx
    {|
      color: red;
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
    |}
  ];

  let discountError = [%cx
    {|
      color: purple;
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
    |}
  ];

  let promoCodeContainer = [%cx
    {|
      display: flex;
      align-items: center;
      justify-content: space-between;
    |}
  ];

  let promoCodeLabel = [%cx
    {|
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
    |}
  ];
};

[@react.component]
let make =
    (
      ~items: list(Item.t),
      ~date: Js.Date.t,
      ~setDiscountValue: float => unit,
    ) => {
  let (code, setCode) = RR.useStateValue("");
  let (submittedCode, setSubmittedCode) = RR.useStateValue(None);

  let discount =
    switch (submittedCode) {
    | None => `NoSubmittedCode
    | Some(code) =>
      switch (Discount.getDiscountFunction(code, date)) {
      | Error(error) => `CodeError(error)
      | Ok(discountFunction) =>
        switch (discountFunction(items)) {
        | Error(error) => `DiscountError(error)
        | Ok(value) => `Discount(value)
        }
      }
    };

  <form
    className=Style.form
    onSubmit={evt => {
      evt |> React.Event.Form.preventDefault;
      setSubmittedCode(Some(code));
    }}>
    <div className=Style.promoCodeContainer>
      <div className=Style.promoCodeLabel> {React.string("Promo code")} </div>
      <input
        className=Style.input
        value=code
        onChange={evt => {
          evt |> RR.getValueFromEvent |> setCode;
          setSubmittedCode(None);
        }}
      />
    </div>
    {switch (discount) {
     | `NoSubmittedCode =>
       setDiscountValue(0.);
       React.null;
     | `Discount(discount) =>
       setDiscountValue(discount);

       <div className=Style.discountValue>
         {discount |> Float.neg |> RR.currency}
       </div>;
     | `CodeError(error) =>
       setDiscountValue(0.);
       <div className=Style.codeError>
         {let errorType =
            switch (error) {
            | Discount.InvalidCode => "Invalid"
            | ExpiredCode => "Expired"
            };
          {j|$errorType promo code|j} |> RR.s}
       </div>;
     | `DiscountError(code) =>
       setDiscountValue(0.);
       let buyWhat =
         switch (code) {
         | `NeedOneBurger => "at least 1 more burger"
         | `NeedTwoBurgers => "at least 2 burgers"
         | `NeedMegaBurger => "a burger with every topping"
         | `MissingSandwichTypes => "every sandwich"
         };
       <div className=Style.discountError>
         {RR.s({j|Buy $buyWhat to enjoy this promotion|j})}
       </div>;
     }}
  </form>;
};
