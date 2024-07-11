open Fest;

module ErrorReducer = {
  test("AddError actually adds a new error to error list", () => {
    let action = Create.AddError("lettuce", "Lettuce cannot be empty");
    let updatedErrors = Create.errorReducer([], action);
    expect
    |> deepEqual(updatedErrors, [("lettuce", "Lettuce cannot be empty")]);
  });

  test("RemoveError actually removes error from error list", () => {
    let action = Create.RemoveError("lettuce");
    let updatedErrors =
      Create.errorReducer([("lettuce", "Lettuce cannot be empty")], action);
    expect |> deepEqual(updatedErrors, []);
  });

  test(
    "RemoveError removes the right error when there are multiple error in error list",
    () => {
      let action = Create.RemoveError("lettuce");
      let updatedErrors =
        Create.errorReducer(
          [
            ("onions", "Onions cannot be negative"),
            ("sandwichType", "Sandwich type cannot be empty"),
            ("lettuce", "Lettuce cannot be empty"),
          ],
          action,
        );
      expect
      |> deepEqual(
           updatedErrors,
           [
             ("onions", "Onions cannot be negative"),
             ("sandwichType", "Sandwich type cannot be empty"),
           ],
         );
    },
  );

  test("ClearErrors removes all errors from error list", () => {
    let action = Create.ClearErrors;
    let updatedErrors =
      Create.errorReducer(
        [
          ("lettuce", "Lettuce cannot be empty"),
          ("sandwichType", "Sandwich type cannot be empty"),
        ],
        action,
      );
    expect |> deepEqual(updatedErrors, []);
  });
};

module ValidateValue = {
  test("NotEmpty rule with none bool value should return error message", () => {
    let output = Create.validateValue(NotEmpty, "Lettuce", BoolValue(None));
    expect |> equal(output, Some("Lettuce cannot be empty"));
  });

  test(
    "NotEmpty rule with non none bool value should not return error message",
    () => {
    let output =
      Create.validateValue(NotEmpty, "Lettuce", BoolValue(Some(true)));
    expect |> equal(output, None);
  });

  test("NotEmpty rule with none int value should return error message", () => {
    let output = Create.validateValue(NotEmpty, "Onions", IntValue(None));
    expect |> equal(output, Some("Onions cannot be empty"));
  });

  test(
    "NotEmpty rule with non none int value should not return error message", () => {
    let output =
      Create.validateValue(NotEmpty, "Onions", IntValue(Some(-5)));
    expect |> equal(output, None);
  });

  test(
    "NotNegative rule with none int value should return NotEmpty error message",
    () => {
    let output = Create.validateValue(NotNegative, "Onions", IntValue(None));
    expect |> equal(output, Some("Onions cannot be empty"));
  });

  test(
    "NotNegative rule with negative int value should return error message", () => {
    let output =
      Create.validateValue(NotNegative, "Onions", IntValue(Some(-5)));
    expect |> equal(output, Some("Onions cannot be negative"));
  });

  test(
    "NotNegative rule with non negative int value should not return error message",
    () => {
    let output =
      Create.validateValue(NotNegative, "Onions", IntValue(Some(0)));
    expect |> equal(output, None);
  });
};

module ValidateValues = {
  test("Single validation passes", () => {
    let (errorToAdd, errorToRemove) =
      Create.validateValues([
        ("lettuce", "Lettuce", NotEmpty, BoolValue(Some(true))),
      ]);
    expect |> deepEqual((errorToAdd, errorToRemove), ([], ["lettuce"]));
  });

  test("Single validation fails", () => {
    let (errorToAdd, errorToRemove) =
      Create.validateValues([
        ("lettuce", "Lettuce", NotEmpty, BoolValue(None)),
      ]);
    expect
    |> deepEqual(
         (errorToAdd, errorToRemove),
         ([("lettuce", "Lettuce cannot be empty")], []),
       );
  });

  test("Multiple validations all pass", () => {
    let (errorToAdd, errorToRemove) =
      Create.validateValues([
        ("lettuce", "Lettuce", NotEmpty, BoolValue(Some(true))),
        ("onions", "Onions", NotNegative, IntValue(Some(1))),
        ("bacon", "Bacon", NotNegative, IntValue(Some(0))),
      ]);
    expect
    |> deepEqual(
         (errorToAdd, errorToRemove),
         ([], ["lettuce", "onions", "bacon"]),
       );
  });

  test("Multiple validations all fail", () => {
    let (errorToAdd, errorToRemove) =
      Create.validateValues([
        ("lettuce", "Lettuce", NotEmpty, BoolValue(None)),
        ("onions", "Onions", NotNegative, IntValue(None)),
        ("bacon", "Bacon", NotNegative, IntValue(Some(-2))),
      ]);
    expect
    |> deepEqual(
         (errorToAdd, errorToRemove),
         (
           [
             ("lettuce", "Lettuce cannot be empty"),
             ("onions", "Onions cannot be empty"),
             ("bacon", "Bacon cannot be negative"),
           ],
           [],
         ),
       );
  });

  test("Multiple validations some pass some fail", () => {
    let (errorToAdd, errorToRemove) =
      Create.validateValues([
        ("lettuce", "Lettuce", NotEmpty, BoolValue(Some(true))),
        ("tomatoes", "Tomatoes", NotEmpty, BoolValue(None)),
        ("onions", "Onions", NotNegative, IntValue(None)),
        ("cheese", "Cheese", NotNegative, IntValue(Some(2))),
        ("bacon", "Bacon", NotNegative, IntValue(Some(-3))),
      ]);
    expect
    |> deepEqual(
         (errorToAdd, errorToRemove),
         (
           [
             ("tomatoes", "Tomatoes cannot be empty"),
             ("onions", "Onions cannot be empty"),
             ("bacon", "Bacon cannot be negative"),
           ],
           ["lettuce", "cheese"],
         ),
       );
  });

  test("Empty list/no validations", () => {
    let (errorToAdd, errorToRemove) = Create.validateValues([]);
    expect |> deepEqual((errorToAdd, errorToRemove), ([], []));
  });
};

module GetErrorMessage = {
  test("Field does not exist in an empty error list", () => {
    let errorMessage = Create.getErrorMessage([], "lettuce");
    expect |> equal(errorMessage, "");
  });

  test("Field exists in a single element error list", () => {
    let errorMessage =
      Create.getErrorMessage(
        [("lettuce", "Lettuce cannot be empty")],
        "lettuce",
      );
    expect |> equal(errorMessage, "Lettuce cannot be empty");
  });

  test("Field does not exist in a single element error list", () => {
    let errorMessage =
      Create.getErrorMessage(
        [("lettuce", "Lettuce cannot be empty")],
        "tomatoes",
      );
    expect |> equal(errorMessage, "");
  });

  test("Field exists in a multiple elements error list", () => {
    let errorMessage =
      Create.getErrorMessage(
        [
          ("lettuce", "Lettuce cannot be empty"),
          ("tomatoes", "Tomatoes cannot be empty"),
          ("cheese", "Cheese cannot be negative"),
        ],
        "tomatoes",
      );
    expect |> equal(errorMessage, "Tomatoes cannot be empty");
  });

  test("Field does not exist in a multiple elements error list", () => {
    let errorMessage =
      Create.getErrorMessage(
        [
          ("lettuce", "Lettuce cannot be empty"),
          ("tomatoes", "Tomatoes cannot be empty"),
          ("cheese", "Cheese cannot be negative"),
        ],
        "bacon",
      );
    expect |> equal(errorMessage, "");
  });
};
