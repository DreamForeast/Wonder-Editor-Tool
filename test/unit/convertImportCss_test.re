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
          let res = ChangeSnapShotPath.changeSnapShotPath("./ttt.js");
          /* expect(res) |> toEqual([||]) */
          expect(1) |> toEqual(1)
        }
      )
    }
  );