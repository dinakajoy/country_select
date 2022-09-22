open Utils;
open Types;

[@react.component]
let make = (~className, ~country, ~onChange) => {
  let (state, setState) = React.useState(() => LoadingCountries);
  let (selectedItem, setSelectedItem) =
    React.useState(() =>
      {
        label: "United States",
        value:
          switch (country) {
          | Some(country) => country
          | None => "us"
          },
      }
    );
  let (searchQuery, setSearchQuery) = React.useState(() => "");
  let (dropdownClass, setDropdownClass) = React.useState(() => false);

  React.useEffect0(() => {
    Js.Promise.(
      fetch(
        "https://gist.githubusercontent.com/rusty-key/659db3f4566df459bd59c8a53dc9f71f/raw/4127f9550ef063121c564025f6d27dceeb279623/counties.json",
      )
      |> then_(response => response##json())
      |> then_(jsonResponse => {
           setState(_previousState => LoadedCountries(jsonResponse));
           Js.Promise.resolve();
         })
      |> catch(err => {
           Js.log(err);
           setState(_previousState => ErrorFetchingCountries);
           Js.Promise.resolve();
         })
      |> ignore
    );

    None;
  });

  let handleDropdownClick = _event => {
    setDropdownClass(previousState => !previousState);
  };

  let handleCountrySelect = (label, value) => {
    onChange({label, value});
    setSelectedItem(_previousState => {label, value});
    setDropdownClass(_previousState => false);
  };

  <section className>
    <div
      className="custom-dropdown"
      onClick=handleDropdownClick
      onKeyPress=handleDropdownClick
      tabIndex=1
      autoFocus=true>
      <span id={selectedItem.value}> {str(selectedItem.label)} </span>
    </div>
    <input
      onChange={event => {
        let value = ReactEvent.Form.target(event)##value;
        setSearchQuery(_value => value);
      }}
      value=searchQuery
      placeholder="Search"
      tabIndex=0
      className={dropdownClass ? "fixed-search display-fixed-search" : "fixed-search"}
    />
    <div
      className={
        dropdownClass
          ? "custom-dropdown__list--expanded" : "custom-dropdown__list"
      }>

        <ul>

            {switch (state) {
             | ErrorFetchingCountries => React.string("An error occurred!")
             | LoadingCountries => React.string("Loading...")
             | LoadedCountries(countries) =>
               // Here, we first check if there is a search keyword before returning the countries
               let trimmedSearchQuery = Js.String2.trim(searchQuery); // Trim query to remove white spaces
               // There is duplicate code here because the types of countries and serchedCountries don't match
               if (trimmedSearchQuery !== "") {
                 let isSubString = (item, _index) =>
                   Js.String2.includes(
                     String.lowercase_ascii(item.label),
                     String.lowercase_ascii(trimmedSearchQuery),
                   );
                 let serchedCountries =
                   Js.Array2.filteri(countries, isSubString);
                 serchedCountries
                 ->Belt.Array.mapWithIndex((_, country) => {
                     let value = country.value;
                     let label = country.label;
                     <li
                       key=value
                       tabIndex=0
                       className="custom-dropdown-list__option"
                       onClick={_e => handleCountrySelect(label, value)}
                       onKeyPress={_e => handleCountrySelect(label, value)}>
                       <span className={"fi fi-" ++ value} />
                       {str(label)}
                     </li>;
                   })
                 ->array;
               } else {
                 countries
                 ->Belt.Array.mapWithIndex((_, country) => {
                     let value = country.value;
                     let label = country.label;
                     <li
                       key=value
                       tabIndex=0
                       className="custom-dropdown-list__option"
                       onClick={_e => handleCountrySelect(label, value)}
                       onKeyPress={_e => handleCountrySelect(label, value)}>
                       <span className={"fi fi-" ++ value} />
                       {str(label)}
                     </li>;
                   })
                 ->array;
               };
             }}
          </ul>
          // />
          //   tabIndex=0
          //   placeholder="Search"
          //   value=searchQuery
          //   }}
          //     setSearchQuery(_value => value);
          //     let value = ReactEvent.Form.target(event)##value;
          //   onChange={event => {
          // <input
      </div>
      // Search Bar
  </section>;
};
