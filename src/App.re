open Utils;

requireCSS("./styles/global.css");

[@react.component]
let make = () => {
  <main>
    <CountrySelect
      className="custom-class"
      country={Some("us")}
      onChange={country => Js.log(country)}
    />
  </main>;
};
