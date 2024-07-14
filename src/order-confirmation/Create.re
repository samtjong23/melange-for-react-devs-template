type optBool = option(bool);
type optInt = option(int);
type optString = option(string);

type optBurgerRecord = {
  lettuce: optBool,
  tomatoes: optBool,
  onions: optInt,
  cheese: optInt,
  bacon: optInt,
};

type itemType =
  | Hotdog
  | Sandwich(option(Item.Sandwich.t))
  | Burger(optBurgerRecord);

type validationRule =
  | NotEmpty
  | NotNegative;

type validationValue =
  | IntValue(optInt)
  | BoolValue(optBool);

type fieldRecord = {
  name: string,
  displayName: string,
  validation: validationRule,
};

type burgerField =
  | Lettuce(fieldRecord)
  | Tomatoes(fieldRecord)
  | Onions(fieldRecord)
  | Cheese(fieldRecord)
  | Bacon(fieldRecord);

let burgerFields = [
  Lettuce({name: "lettuce", displayName: "Lettuce", validation: NotEmpty}),
  Tomatoes({name: "tomatoes", displayName: "Tomatoes", validation: NotEmpty}),
  Onions({name: "onions", displayName: "Onions", validation: NotNegative}),
  Cheese({name: "cheese", displayName: "Cheese", validation: NotNegative}),
  Bacon({name: "bacon", displayName: "Bacon", validation: NotNegative}),
];

type errorAction =
  | AddError(string, string)
  | RemoveError(string)
  | ClearErrors;

let errorReducer = (errors, errAction) =>
  switch (errAction) {
  | AddError(fieldName, errorMessage) =>
    errors @ [(fieldName, errorMessage)]
  | RemoveError(fieldName) =>
    errors |> List.filter(((field, _)) => field != fieldName)
  | ClearErrors => []
  };

/** For checking if a field contains a valid value
Returns None if value is valid else returns error message */
let validateValue = (rule, displayName, value) =>
  switch (rule, value) {
  | (NotEmpty, IntValue(None))
  | (NotEmpty, BoolValue(None))
  // Not negative also implies not empty
  | (NotNegative, IntValue(None)) => Some(displayName ++ " cannot be empty")
  | (NotNegative, IntValue(Some(intVal))) when intVal < 0 =>
    Some(displayName ++ " cannot be negative")
  | _ => None
  };

/** For checking if a list of values are valid
Returns two lists ie. one for errors to add to the error list
and another for errors to remove from error list */
let validateValues = validations => {
  ListLabels.fold_left(
    ~init=([], []),
    ~f=
      (
        (errorsToAdd, errorsToRemove),
        (fieldName, displayName, validation, value),
      ) =>
        switch (validateValue(validation, displayName, value)) {
        | Some(errorMessage) => (
            errorsToAdd @ [(fieldName, errorMessage)],
            errorsToRemove,
          )
        | None => (errorsToAdd, errorsToRemove @ [fieldName])
        },
    validations,
  );
};

let getErrorMessage = (errors, fieldName): string =>
  switch (List.filter(((field, _)) => field == fieldName, errors)) {
  | [(_, errorMessage)] => errorMessage
  | _ => ""
  };

module Style = {
  let form = [%cx
    {|
      width: 100%;
      display: flex;
      flex-direction: column;
      background-color: #ffffff;
      padding-left: 1rem;
      padding-right: 1rem;
      padding-top: 0.5rem;
      padding-bottom: 1rem;
      border-radius: 0.375rem;
      box-sizing: border-box;
    |}
  ];

  let label = [%cx
    {|
      display: block;
      font-size: 0.875rem;
      font-weight: bold;
      font-family: 'Inter', sans-serif;
      line-height: 1.25rem;
      color: #1f2937;
      padding-top: 0.5rem;
      padding-bottom: 0.5rem;
    |}
  ];

  let select = [%cx
    {|
      display: block;
      width: 100%;
      border-radius: 0.375rem;
      border: 1px solid #d1d5db;
      padding: 0.5rem;
      color: #1f2937;
    |}
  ];

  let input = [%cx
    {|
      padding: 0.5rem;
      border: 1px solid #d1d5db;
      border-radius: 0.375rem;
      width: 80%;
    |}
  ];

  let fieldset = [%cx
    {|
      padding: 0;
      border: 0;
      margin: 0;
    |}
  ];

  let legend = [%cx
    {|
      display: block;
      font-size: 0.875rem;
      font-weight: bold;
      font-family: 'Inter', sans-serif;
      line-height: 1.25rem;
      color: #1f2937;
      padding-top: 1.5rem;
      padding-bottom: 0.5rem;
    |}
  ];

  let radioContainer = [%cx
    {|
      display: flex;
      align-items: center;
      gap: 2rem;
    |}
  ];

  let radioLabel = [%cx
    {|
      padding-left: 0.5rem;
      font-family: 'Inter', sans-serif;
      color: #1f2937;
      font-size: 0.875rem;
    |}
  ];

  let errorText = [%cx
    {|
      color: #ef4444;
      font-size: 0.875rem;
      font-family: 'Inter', sans-serif;
      margin-top: 0.25rem;
    |}
  ];

  let gridContainer = [%cx
    {|
      display: grid;
      grid-template-columns: repeat(6, 1fr);
      gap: 0;
    |}
  ];

  let colSpan3 = [%cx {|
      grid-column: span 3;
    |}];

  let colSpan2 = [%cx {|
      grid-column: span 2;
    |}];

  let submitButtonContainer = [%cx
    {|
      display: flex;
      justify-content: flex-end;
      margin-top: 1.5rem;
      width: 100%;
    |}
  ];

  let submitButton = [%cx
    {|
      padding: 0.5rem 1rem;
      border: 1px solid #d1d5db;
      background-color: #3b82f6;
      color: #ffffff;
      font-weight: 500;
      border-radius: 0.375rem;
      &:hover {
        background-color: #2563eb;
      }
    |}
  ];
};

[@react.component]
let make = (~items: Order.t, ~setItems: Order.t => unit) => {
  let (item, setItem) = RR.useStateValue(None);
  let (errors, dispatchErrorAction) = React.useReducer(errorReducer, []);

  let handleSubmit = evt => {
    evt |> React.Event.Form.preventDefault;

    let updateItems = newItem => {
      setItems(items @ [newItem]);
      setItem(None);
      dispatchErrorAction(ClearErrors);
    };

    // To make sure when new errors are added, there are no duplicates
    dispatchErrorAction(ClearErrors);

    switch (item) {
    | Some(Hotdog) => updateItems(Item.Hotdog)
    | Some(Sandwich(Some(sandwichType))) =>
      updateItems(Item.Sandwich(sandwichType))
    | Some(Sandwich(None)) =>
      dispatchErrorAction(
        AddError("sandwichType", "Sandwich type cannot be empty"),
      )
    | Some(Burger({lettuce, tomatoes, onions, cheese, bacon})) =>
      let validations =
        burgerFields
        |> List.map(field =>
             switch (field) {
             | Lettuce({name, displayName, validation}) => (
                 name,
                 displayName,
                 validation,
                 BoolValue(lettuce),
               )
             | Tomatoes({name, displayName, validation}) => (
                 name,
                 displayName,
                 validation,
                 BoolValue(tomatoes),
               )
             | Onions({name, displayName, validation}) => (
                 name,
                 displayName,
                 validation,
                 IntValue(onions),
               )
             | Cheese({name, displayName, validation}) => (
                 name,
                 displayName,
                 validation,
                 IntValue(cheese),
               )
             | Bacon({name, displayName, validation}) => (
                 name,
                 displayName,
                 validation,
                 IntValue(bacon),
               )
             }
           );

      let (errorsToAdd, errorsToRemove) = validateValues(validations);

      switch (errorsToAdd) {
      | [] =>
        updateItems(
          Item.Burger({
            lettuce: RR.getWithDefault(lettuce, false),
            tomatoes: RR.getWithDefault(tomatoes, false),
            onions: RR.getWithDefault(onions, 0),
            cheese: RR.getWithDefault(cheese, 0),
            bacon: RR.getWithDefault(bacon, 0),
          }),
        )
      | _ =>
        List.iter(
          ((fieldName, errorMessage)) => {
            dispatchErrorAction(AddError(fieldName, errorMessage))
          },
          errorsToAdd,
        )
      };

      List.iter(
        fieldName => {dispatchErrorAction(RemoveError(fieldName))},
        errorsToRemove,
      );
    | None =>
      dispatchErrorAction(AddError("itemType", "Item type cannot be empty"))
    };
  };

  let handleItemTypeChange = itemTypeValue => {
    let newItem =
      switch (itemTypeValue) {
      | "sandwich" => Some(Sandwich(None))
      | "burger" =>
        Some(
          Burger({
            lettuce: None,
            tomatoes: None,
            onions: None,
            cheese: None,
            bacon: None,
          }),
        )
      | "hotdog" => Some(Hotdog)
      | _ => item
      };

    setItem(newItem);
    dispatchErrorAction(ClearErrors);
  };

  let handleSandwichTypeChange = sandwichTypeValue => {
    let newSandwichType =
      switch (sandwichTypeValue) {
      | "portobello" => Some(Item.Sandwich.Portobello)
      | "ham" => Some(Ham)
      | "unicorn" => Some(Unicorn)
      | "turducken" => Some(Turducken)
      | _ => None
      };

    setItem(Some(Sandwich(newSandwichType)));
  };

  let handleBoolFieldChange = (field, value) => {
    switch (item) {
    | Some(Burger(burgerRecord)) =>
      let updatedBurgerRecord =
        switch (field) {
        | "lettuce" => {...burgerRecord, lettuce: value}
        | "tomatoes" => {...burgerRecord, tomatoes: value}
        | _ => burgerRecord
        };

      setItem(Some(Burger(updatedBurgerRecord)));
    | _ => setItem(item)
    };
  };

  let handleIntFieldChange = (field, value) => {
    let intValue = int_of_string_opt(value);

    switch (item) {
    | Some(Burger(burgerRecord)) =>
      let updatedBurgerRecord =
        switch (field) {
        | "onions" => {...burgerRecord, onions: intValue}
        | "cheese" => {...burgerRecord, cheese: intValue}
        | "bacon" => {...burgerRecord, bacon: intValue}
        | _ => burgerRecord
        };

      setItem(Some(Burger(updatedBurgerRecord)));
    | _ => setItem(item)
    };
  };

  <div>
    <form className=Style.form onSubmit=handleSubmit>
      <div>
        <div>
          <label className=Style.label> {RR.s("Choose item type")} </label>
          <div>
            <select
              className=Style.select
              value={
                switch (item) {
                | Some(Hotdog) => "hotdog"
                | Some(Sandwich(_)) => "sandwich"
                | Some(Burger(_)) => "burger"
                | None => ""
                }
              }
              onChange={evt => {
                evt |> RR.getValueFromEvent |> handleItemTypeChange
              }}>
              <option value="" disabled=true>
                {RR.s("Select an item type")}
              </option>
              <option value="hotdog"> {RR.s({js|Hotdog 🌭|js})} </option>
              <option value="sandwich">
                {RR.s({js|Sandwich 🥪|js})}
              </option>
              <option value="burger"> {RR.s({js|Burger 🍔|js})} </option>
            </select>
          </div>
        </div>
        {switch (item) {
         | Some(Sandwich(_)) =>
           <div>
             <label className=Style.label>
               {RR.s("Choose sandwich type")}
             </label>
             <div>
               <select
                 className=Style.select
                 value={
                   switch (item) {
                   | Some(Sandwich(Some(Portobello))) => "portobello"
                   | Some(Sandwich(Some(Ham))) => "ham"
                   | Some(Sandwich(Some(Unicorn))) => "unicorn"
                   | Some(Sandwich(Some(Turducken))) => "turducken"
                   | _ => ""
                   }
                 }
                 onChange={evt => {
                   evt |> RR.getValueFromEvent |> handleSandwichTypeChange
                 }}>
                 <option value="" disabled=true>
                   {RR.s("Select a sandwich type")}
                 </option>
                 <option value="portobello">
                   {RR.s({js|Portobello 🍄|js})}
                 </option>
                 <option value="ham"> {RR.s({js|Ham 🐷|js})} </option>
                 <option value="unicorn">
                   {RR.s({js|Unicorn 🦄|js})}
                 </option>
                 <option value="turducken">
                   {RR.s({js|Turducken 🦃🦆🐓|js})}
                 </option>
               </select>
             </div>
             <p className=Style.errorText>
               {RR.s(getErrorMessage(errors, "sandwichType"))}
             </p>
           </div>
         | Some(Burger({lettuce, tomatoes, onions, cheese, bacon})) =>
           <div className=Style.gridContainer>
             <div className=Style.colSpan3>
               <fieldset className=Style.fieldset>
                 <legend className=Style.legend> {RR.s("Lettuce")} </legend>
                 <div className=Style.radioContainer>
                   <div>
                     <input
                       type_="radio"
                       checked={lettuce == Some(true)}
                       onChange={_ =>
                         handleBoolFieldChange("lettuce", Some(true))
                       }
                     />
                     <span className=Style.radioLabel> {RR.s("Yes")} </span>
                   </div>
                   <div>
                     <input
                       type_="radio"
                       checked={lettuce == Some(false)}
                       onChange={_ =>
                         handleBoolFieldChange("lettuce", Some(false))
                       }
                     />
                     <span className=Style.radioLabel> {RR.s("No")} </span>
                   </div>
                 </div>
                 <p className=Style.errorText>
                   {RR.s(getErrorMessage(errors, "lettuce"))}
                 </p>
               </fieldset>
             </div>
             <div className=Style.colSpan3>
               <fieldset className=Style.fieldset>
                 <legend className=Style.legend> {RR.s("Tomatoes")} </legend>
                 <div className=Style.radioContainer>
                   <div>
                     <input
                       type_="radio"
                       checked={tomatoes == Some(true)}
                       onChange={_ =>
                         handleBoolFieldChange("tomatoes", Some(true))
                       }
                     />
                     <span className=Style.radioLabel> {RR.s("Yes")} </span>
                   </div>
                   <div>
                     <input
                       type_="radio"
                       checked={tomatoes == Some(false)}
                       onChange={_ =>
                         handleBoolFieldChange("tomatoes", Some(false))
                       }
                     />
                     <span className=Style.radioLabel> {RR.s("No")} </span>
                   </div>
                 </div>
                 <p className=Style.errorText>
                   {RR.s(getErrorMessage(errors, "tomatoes"))}
                 </p>
               </fieldset>
             </div>
             <div className=Style.colSpan2>
               <label className=Style.label> {RR.s("Onions")} </label>
               <input
                 className=Style.input
                 type_="number"
                 step=1.
                 value={RR.optionIntToString(onions)}
                 placeholder="Enter number of onions"
                 onChange={evt => {
                   evt
                   |> RR.getValueFromEvent
                   |> handleIntFieldChange("onions")
                 }}
               />
               <p className=Style.errorText>
                 {RR.s(getErrorMessage(errors, "onions"))}
               </p>
             </div>
             <div className=Style.colSpan2>
               <label className=Style.label> {RR.s("Cheese")} </label>
               <input
                 className=Style.input
                 type_="number"
                 step=1.
                 value={RR.optionIntToString(cheese)}
                 placeholder="Enter number of cheese slices"
                 onChange={evt => {
                   evt
                   |> RR.getValueFromEvent
                   |> handleIntFieldChange("cheese")
                 }}
               />
               <p className=Style.errorText>
                 {RR.s(getErrorMessage(errors, "cheese"))}
               </p>
             </div>
             <div className=Style.colSpan2>
               <label className=Style.label> {RR.s("Bacon")} </label>
               <input
                 className=Style.input
                 type_="number"
                 step=1.
                 value={RR.optionIntToString(bacon)}
                 placeholder="Enter number of bacon strips"
                 onChange={evt => {
                   evt
                   |> RR.getValueFromEvent
                   |> handleIntFieldChange("bacon")
                 }}
               />
               <p className=Style.errorText>
                 {RR.s(getErrorMessage(errors, "bacon"))}
               </p>
             </div>
           </div>
         | _ =>
           <p className=Style.errorText>
             {RR.s(getErrorMessage(errors, "itemType"))}
           </p>
         }}
        <div className=Style.submitButtonContainer>
          <button className=Style.submitButton> {RR.s("Add")} </button>
        </div>
      </div>
    </form>
  </div>;
};
