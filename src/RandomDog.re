type dog = string;
type apiData('a) = RemoteData.t('a, string, string);

type action =
  | FetchData
  | FetchFailed(string)
  | FetchSuccess(dog);

type state = {data: apiData(dog)};

module Decode = {
  let dog = json => Json.Decode.(field("message", string, json));
};

let showMeADog = send => {
  Js.Promise.(
    Fetch.fetch("https://dog.ceo/api/breeds/image/random")
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json |> Decode.dog |> (dog => send(FetchSuccess(dog))) |> resolve
       )
    |> catch(_err => send(FetchFailed([%bs.raw "_err.message"])) |> resolve)
    |> ignore
  );
};

let component = ReasonReact.reducerComponent("RandomDog");
let str = ReasonReact.string;

let make = _children => {
  ...component,

  initialState: () => {data: NotAsked},
  didMount: ({send}) => {
    send(FetchData);
  },

  reducer: (action, _state) => {
    switch (action) {
    | FetchData =>
      ReasonReact.UpdateWithSideEffects(
        {data: Loading("Loading...")},
        self => showMeADog(self.send),
      )
    | FetchFailed(e) => ReasonReact.Update({data: Failure(e)})
    | FetchSuccess(dog) => ReasonReact.Update({data: Success(dog)})
    };
  },

  render: self => {
    switch (self.state.data) {
    | NotAsked => ReasonReact.null
    | Loading(l) => str(l)
    | Failure(e) => str(e)
    | Success(dog) => <img src=dog />
    };
  },
};
