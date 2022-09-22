let str = React.string;
let array = React.array;

[@bs.val] external requireCSS: string => unit = "require";

[@bs.val] external fetch: string => Js.Promise.t('a) = "fetch";
