let changePathCode = "const getSnapshotPath = exports.getSnapshotPath = testPath => {\n    let filePathArray = testPath.split('/');\n    filePathArray.splice(5, 2);\n    testPath = filePathArray.join('/');\n    return _path2.default.join(\n      _path2.default.join(_path2.default.dirname(testPath), '__snapshots__'),\n      _path2.default.basename(testPath) + '.' + SNAPSHOT_EXTENSION)\n  };";

let changeSnapShotPath = (filePath) => {
  let pathRe = [%re {|/const\s*getSnapshotPath.*([^.].*){4}/g|}];
  Node.Fs.readFileSync(filePath, `utf8)
  |> (
    (value) =>
      switch (pathRe |> Js.Re.exec(value)) {
      | None => value
      | Some(val_) =>
        let res = Js.Re.matches(val_)[0];
        let len = Js.String.length(res);
        let index = Js.String.indexOf(res, value);
        let start = Js.String.slice(~from=0, ~to_=index, value);
        let end_ = Js.String.slice(~from=index + len, ~to_=value |> Js.String.length, value);
        start ++ changePathCode ++ end_
      }
  )
  |> (
    (resultFile) => {
      Node.Fs.writeFileSync(~filename=filePath, ~text=resultFile);
      Js.log("change snapshot path success : " ++ filePath)
    }
  )
};