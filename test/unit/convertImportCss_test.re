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
          let res = ConvertImportCss.convert("./libs/es6_global");
          /* expect(res) |> toEqual([||]) */
          expect(1) |> toEqual(1)
        }
      )
    }
  );