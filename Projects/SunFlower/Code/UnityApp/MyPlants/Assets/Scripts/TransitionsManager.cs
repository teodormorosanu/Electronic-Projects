using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TransitionsManager : MonoBehaviour {
    [SerializeField] private Animator swipe;

    public void LoadScene() {
        StartCoroutine(LoadSceneCoroutine(SceneManager.GetActiveScene().buildIndex + 1));
    }

    private IEnumerator LoadSceneCoroutine(int levelIndex) {
        swipe.SetTrigger("start");
        yield return new WaitForSeconds(.5f);
        SceneManager.LoadScene(levelIndex);
    }
}
