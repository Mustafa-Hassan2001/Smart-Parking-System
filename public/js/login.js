// Initialize the FirebaseUI Widget using Firebase.
// var ui = new firebaseui.auth.AuthUI(firebase.auth());


// firebase.auth.Auth.Persistence.LOCAL

// $("loginbtn").click(function()
// {
//     var UserEmail = document.getElementById("Email_field").value;
//     var UserPass = document.getElementById("Password_field").value;

//     var result = firebase.auth().signInWithEmailAndPassword(UserEmail, UserPass)
//     window.alert('nnnnn');
//     result.catch(function(error)
//     {
//         var errorCode = error.code;
//         var errorMessage = error.message;
//         console.log(errorCode);
//         window.alert('Error for: '+errorMessage+UserEmail+" "+UserPass);
//     });
// });


// firebase.auth().onAuthStateChanged(function(user) {
//     if (user) {
//       // User is signed in.
//       window.alert('inside');
//       //window.location.assign('./schedule.html')
//     } else {
//       // No user is signed in.
//       window.alert('outtttt');
//     }
//   });






  
//   const auth = firebase.auth();
//   function login(){
//     var UserEmail = document.getElementById("Email_field").value;
//     var UserPass = document.getElementById("Password_field").value;
    
//     const promise =auth.signInWithEmailAndPassword(UserEmail, UserPass);
//     promise.catch(e => alert(e.message));
//     alert("signed in as "+UserEmail);
//     window.location.assign('about.html')
    
//   }
//     const SingInWithEmailFunction = () => {
//         const email = UserEmail;
//         const password = UserPass;
//         auth.signInWithEmailAndPassword(UserEmail, UserPass)
//         .then(()=>{
//             windows.alert('there u r');
//         })
//         .catch(error=>{
//             console.error(error);
//         })
//     }


//     }
//     firebase.auth().signInWithEmailAndPassword(UserEmail, UserPass)
//     .catch(function(error) {
//   // Handle Errors here.
//   var errorCode = error.code;
//   var errorMessage = error.message;
//   if (errorCode === 'auth/wrong-password') {
//     alert('Wrong password.');
//   } else {
//     alert(errorMessage);
//   }
//   console.log(error);
// });



//     firebase.auth().signInWithEmailAndPassword(UserEmail, UserPass)
//   .then((userCredential) => {
//     // Signed in
//     window.alert('Logged in!');
//     var user = userCredential.user;
//     // ...
//   })
//   .catch((error) => {
//     var errorCode = error.code;
//     var errorMessage = error.message;
//     window.alert('Error for: '+errorMessage+UserEmail+" "+UserPass);
//   });
//   }