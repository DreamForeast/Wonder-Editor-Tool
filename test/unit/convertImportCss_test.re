open Wonder_jest;

let _ =
  describe(
    "create shunk system file",
    () => {
      open Expect;
      open Expect.Operators;
      open Sinon;
      let sandbox = getSandboxDefaultVal();
      beforeEach(() => sandbox := createSandbox());
      /* afterEach(); */
      test(
        "add private functions",
        () => {
          let res = ConverImportCss.convert("./libs/es6_global/src");
          /* expect(res) |> toEqual([||]) */
          expect(1) |> toEqual(1)
        }
      )
    }
  );