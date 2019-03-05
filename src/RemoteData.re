type t('a, 'l, 'e) =
  | NotAsked
  | Loading('l)
  | Failure('e)
  | Success('a);

let map = (f, data) =>
  switch (data) {
  | Success(a) => Success(f(a))
  | _ => data
  };

let andThen = (f, data) => {
  switch (data) {
  | Success(a) => f(a)
  | _ => data
  };
};

let toOption =
  fun
  | Success(a) => Some(a)
  | _ => None;
