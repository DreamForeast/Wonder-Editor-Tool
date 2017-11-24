open FsExtend;

let shouldRemovePathArr = [|"lib", "es6_global"|];

let _removeRedundancePath = (path) =>
  path |> Js.String.slice(~from=3, ~to_=path |> Js.String.length);

let _getRelativePath = (filePath) =>
  filePath
  |> Js.String.split("/")
  |> Js.Array.filter((pathSegment) => ! (shouldRemovePathArr |> Js.Array.includes(pathSegment)))
  |> Js.Array.slice(~start=0, ~end_=(-1))
  |> Js.Array.joinWith("/")
  |> PathExtend.relative(filePath)
  |> _removeRedundancePath;

let changeImportCss = (relativePath, fileContent) => {
  let importCssRe = [%re {|/^importCss\s*\((\"|\')/g|}];
  let fileNameRe = [%re {|/[\"|\'](.*?)[\"|\']/g|}];
  let hasChangeFile = ref(false);
  fileContent
  |> Js.String.split("\n")
  |> Js.Array.reduce(
       (resultLine, line) =>
         switch (importCssRe |> Js.Re.exec(line)) {
         | None => resultLine ++ line ++ "\n"
         | Some(result) =>
           hasChangeFile := true;
           switch (fileNameRe |> Js.Re.exec(line)) {
           | None => resultLine
           | Some(res_) =>
             let fileNameResult = Js.Re.matches(res_);
             let index = fileNameResult[0] |> Js.String.indexOf("/");
             fileNameResult[0]
             |> Js.String.slice(~from=index + 1, ~to_=(-1))
             |> (
               (fileName) => resultLine ++ "import '" ++ relativePath ++ "/" ++ fileName ++ "';\n"
             )
           }
         },
       ""
     )
  |> ((resultFile) => (hasChangeFile, resultFile))
};

let parseSystem = (filePath) => {
  let relativePath = filePath |> _getRelativePath;
  Node.Fs.readFileSync(filePath, `utf8)
  |> changeImportCss(relativePath)
  |> (
    ((hasChangeFile, resultFile)) =>
      if (hasChangeFile^) {
        Node.Fs.writeFileSync(~filename=filePath, ~text=resultFile);
        Js.log("success : " ++ filePath)
      }
  )
};

let rec convert = (filePath) =>
  filePath
  |> isFilePathExist
  |> Node.Fs.readdirSync
  |> Js.Array.forEach(
       (fileName: string) => {
         let fileDir = fileName |> PathExtend.join(filePath);
         fileDir
         |> statSync
         |> (
           (stats) =>
             if (statsToJsObj(stats)##isFile()) {
               fileDir
               |> PathExtend.parse
               |> (
                 (pathObj) => {
                   let ext = PathExtend.parseTojsObj(pathObj)##ext;
                   switch ext {
                   | ".js" => fileDir |> parseSystem
                   | _ => ()
                   }
                 }
               )
             } else if (statsToJsObj(stats)##isDirectory()) {
               convert(fileName |> PathExtend.join(filePath))
             }
         )
       }
     );